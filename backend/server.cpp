/*server.cpp*/

#include "httplib.h"
#include "api_helpers.h"
#include "building.h"
#include "buildings.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "busstop.h"
#include "busstops.h"
#include "curl_util.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <memory>

using namespace std;
using namespace tinyxml2;
using json = nlohmann::json;

// Global state (loaded once at startup)
XMLDocument xmldoc;
unique_ptr<Nodes> nodes;
unique_ptr<Buildings> buildings;
unique_ptr<BusStops> busStops;
string apiKey = "kyLuFuPPDav4yBC7gCJ2RFz58";

bool loadData(const string& osmFilename, const string& busStopsFilename) {
    // Load OSM file
    if (!osmLoadMapFile(osmFilename, xmldoc)) {
        cerr << "ERROR: Failed to load OSM file: " << osmFilename << endl;
        return false;
    }
    
    // Initialize nodes and buildings
    nodes = make_unique<Nodes>();
    buildings = make_unique<Buildings>();
    
    nodes->readMapNodes(xmldoc);
    buildings->readMapBuildings(xmldoc);
    
    // Load bus stops
    busStops = make_unique<BusStops>(busStopsFilename);
    
    // Note: CURL handles are created per-request for thread safety
    // No need to initialize a global curl handle
    
    cout << "Data loaded successfully:" << endl;
    cout << "  Nodes: " << nodes->getNumMapNodes() << endl;
    cout << "  Buildings: " << buildings->getNumMapBuildings() << endl;
    cout << "  Bus stops: " << busStops->StopList.size() << endl;
    
    return true;
}

void cleanup() {
    // No global curl handle to clean up - handles are created per-request
}

int main(int argc, char* argv[]) {
    // Default filenames
    string osmFilename = "nu.osm";
    string busStopsFilename = "bus-stops.txt";
    
    // Allow override via command line
    if (argc >= 2) {
        osmFilename = argv[1];
    }
    if (argc >= 3) {
        busStopsFilename = argv[2];
    }
    
    // Load data
    if (!loadData(osmFilename, busStopsFilename)) {
        return 1;
    }
    
    // Create HTTP server
    httplib::Server svr;
    
    // Enable CORS
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    
    // Handle OPTIONS requests for CORS
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        return;
    });
    
    // Health check endpoint
    svr.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        json response;
        response["status"] = "ok";
        response["nodes"] = nodes->getNumMapNodes();
        response["buildings"] = buildings->getNumMapBuildings();
        response["busStops"] = busStops->StopList.size();
        res.set_content(response.dump(), "application/json");
    });
    
    // Get all buildings
    svr.Get("/api/buildings", [](const httplib::Request&, httplib::Response& res) {
        json response = buildingsToJson(*buildings, *nodes);
        res.set_content(response.dump(), "application/json");
    });
    
    // Search buildings by name
    svr.Get("/api/buildings/search", [](const httplib::Request& req, httplib::Response& res) {
        string name = req.get_param_value("name");
        if (name.empty()) {
            json error;
            error["error"] = "Missing 'name' parameter";
            res.status = 400;
            res.set_content(error.dump(), "application/json");
            return;
        }
        
        // Create a new CURL handle for this request (thread-safe)
        CURL* requestCurl = curl_easy_init();
        if (requestCurl == nullptr) {
            json error;
            error["error"] = "Failed to initialize CURL";
            res.status = 500;
            res.set_content(error.dump(), "application/json");
            return;
        }
        
        json response = searchBuildingsJson(name, *buildings, *nodes, *busStops, requestCurl, apiKey);
        curl_easy_cleanup(requestCurl);
        res.set_content(response.dump(), "application/json");
    });
    
    // Get all bus stops
    svr.Get("/api/bus-stops", [](const httplib::Request&, httplib::Response& res) {
        json response = busStopsToJson(*busStops);
        res.set_content(response.dump(), "application/json");
    });
    
    // Find nearby bus stops by coordinates
    svr.Get("/api/bus-stops/nearby", [](const httplib::Request& req, httplib::Response& res) {
        string latStr = req.get_param_value("lat");
        string lonStr = req.get_param_value("lon");
        
        if (latStr.empty() || lonStr.empty()) {
            json error;
            error["error"] = "Missing 'lat' or 'lon' parameter";
            res.status = 400;
            res.set_content(error.dump(), "application/json");
            return;
        }
        
        try {
            double lat = stod(latStr);
            double lon = stod(lonStr);
            
            // Create a new CURL handle for this request (thread-safe)
            CURL* requestCurl = curl_easy_init();
            if (requestCurl == nullptr) {
                json error;
                error["error"] = "Failed to initialize CURL";
                res.status = 500;
                res.set_content(error.dump(), "application/json");
                return;
            }
            
            json response = nearbyStopsJson(lat, lon, *busStops, requestCurl, apiKey);
            curl_easy_cleanup(requestCurl);
            res.set_content(response.dump(), "application/json");
        } catch (const exception& e) {
            json error;
            error["error"] = "Invalid latitude or longitude";
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });
    
    // Get bus predictions for a specific stop
    svr.Get("/api/predictions", [](const httplib::Request& req, httplib::Response& res) {
        string stopIdStr = req.get_param_value("stopId");
        string route = req.get_param_value("route");
        
        if (stopIdStr.empty() || route.empty()) {
            json error;
            error["error"] = "Missing 'stopId' or 'route' parameter";
            res.status = 400;
            res.set_content(error.dump(), "application/json");
            return;
        }
        
        try {
            long long stopId = stoll(stopIdStr);
            
            // Create a new CURL handle for this request (thread-safe)
            CURL* requestCurl = curl_easy_init();
            if (requestCurl == nullptr) {
                json error;
                error["error"] = "Failed to initialize CURL";
                res.status = 500;
                res.set_content(error.dump(), "application/json");
                return;
            }
            
            json response = getBusPredictionsJson(stopId, route, requestCurl, apiKey);
            curl_easy_cleanup(requestCurl);
            res.set_content(response.dump(), "application/json");
        } catch (const exception& e) {
            json error;
            error["error"] = "Invalid stopId";
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    });
    
    // Start server
    int port = 8080;
    cout << "Starting server on port " << port << "..." << endl;
    cout << "API endpoints:" << endl;
    cout << "  GET /api/health" << endl;
    cout << "  GET /api/buildings" << endl;
    cout << "  GET /api/buildings/search?name=<name>" << endl;
    cout << "  GET /api/bus-stops" << endl;
    cout << "  GET /api/bus-stops/nearby?lat=<lat>&lon=<lon>" << endl;
    cout << "  GET /api/predictions?stopId=<id>&route=<route>" << endl;
    
    if (!svr.listen("127.0.0.1", port)) {
        cerr << "ERROR: Failed to start server" << endl;
        cleanup();
        return 1;
    }
    
    cleanup();
    return 0;
}

