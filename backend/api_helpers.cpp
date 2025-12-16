/*api_helpers.cpp*/

#include "api_helpers.h"
#include "dist.h"
#include <algorithm>
#include <cctype>

using namespace std;
using json = nlohmann::json;

json buildingToJson(const Building& building, const Nodes& nodes) {
    json j;
    j["id"] = building.ID;
    j["name"] = building.Name;
    j["streetAddress"] = building.StreetAddress;
    
    auto location = building.getLocation(nodes);
    j["location"] = json::object();
    j["location"]["latitude"] = location.first;
    j["location"]["longitude"] = location.second;
    
    return j;
}

json buildingsToJson(const Buildings& buildings, const Nodes& nodes) {
    json j = json::array();
    for (const Building& building : buildings.MapBuildings) {
        j.push_back(buildingToJson(building, nodes));
    }
    return j;
}

json busStopToJson(const BusStop& stop) {
    json j;
    j["id"] = stop.ID;
    j["route"] = stop.Route;
    j["stopName"] = stop.StopName;
    j["direction"] = stop.Direction;
    j["location"] = stop.Location;
    j["latitude"] = stop.Latitude;
    j["longitude"] = stop.Longitude;
    return j;
}

json busStopsToJson(const BusStops& busStops) {
    json j = json::array();
    for (const BusStop& stop : busStops.StopList) {
        j.push_back(busStopToJson(stop));
    }
    return j;
}

json getBusPredictionsJson(long long stopId, const string& route, CURL* curl, const string& apiKey) {
    json result = json::object();
    result["stopId"] = stopId;
    result["route"] = route;
    result["predictions"] = json::array();
    
    if (curl == nullptr) {
        result["error"] = "CURL handle is null";
        return result;
    }
    
    string url = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=" + apiKey +
                 "&rt=" + route + "&stpid=" + to_string(stopId) + "&format=json";
    
    string response;
    bool success = callWebServer(curl, url, response);
    
    if (!success) {
        // Return empty predictions array instead of error
        return result;
    }
    
    try {
        auto jsondata = json::parse(response);
        auto busResponse = jsondata["bustime-response"];
        
        if (busResponse.find("prd") != busResponse.end()) {
            auto predictions = busResponse["prd"];
            for (auto& prediction : predictions) {
                json pred;
                try {
                    pred["vehicleId"] = stoi(prediction["vid"].get_ref<string&>());
                    pred["route"] = prediction["rt"].get<string>();
                    pred["direction"] = prediction["rtdir"].get<string>();
                    string time = prediction["prdctdn"].get<string>();
                    if (time == "DUE") {
                        time = "due";
                    } else if (time.empty()) {
                        time = "unknown";
                    }
                    pred["arrivalTime"] = time;
                    result["predictions"].push_back(pred);
                } catch (const exception& e) {
                    // Skip malformed predictions
                }
            }
        } else {
            result["error"] = "No predictions available";
        }
    } catch (const exception& e) {
        result["error"] = "Failed to parse CTA API response";
    }
    
    return result;
}

json nearbyStopsJson(double lat, double lon, const BusStops& busStops, CURL* curl, const string& apiKey) {
    json result = json::object();
    result["location"] = json::object();
    result["location"]["latitude"] = lat;
    result["location"]["longitude"] = lon;
    result["stops"] = json::object();
    
    auto closestStops = busStops.findClosestStops(lat, lon);
    
    if (closestStops.first) {
        json northbound = busStopToJson(*closestStops.first);
        double distance = distBetween2Points(lat, lon, closestStops.first->Latitude, closestStops.first->Longitude);
        northbound["distance"] = distance;
        northbound["predictions"] = getBusPredictionsJson(closestStops.first->ID, closestStops.first->Route, curl, apiKey)["predictions"];
        result["stops"]["northbound"] = northbound;
    }
    
    if (closestStops.second) {
        json southbound = busStopToJson(*closestStops.second);
        double distance = distBetween2Points(lat, lon, closestStops.second->Latitude, closestStops.second->Longitude);
        southbound["distance"] = distance;
        southbound["predictions"] = getBusPredictionsJson(closestStops.second->ID, closestStops.second->Route, curl, apiKey)["predictions"];
        result["stops"]["southbound"] = southbound;
    }
    
    return result;
}

json searchBuildingsJson(const string& name, const Buildings& buildings, const Nodes& nodes, const BusStops& busStops, CURL* curl, const string& apiKey) {
    json result = json::array();
    
    string searchNameLower = name;
    transform(searchNameLower.begin(), searchNameLower.end(), searchNameLower.begin(), ::tolower);
    
    for (const Building& building : buildings.MapBuildings) {
        string buildingNameLower = building.Name;
        transform(buildingNameLower.begin(), buildingNameLower.end(), buildingNameLower.begin(), ::tolower);
        
        if (buildingNameLower.find(searchNameLower) != string::npos) {
            json buildingJson = buildingToJson(building, nodes);
            
            auto location = building.getLocation(nodes);
            auto closestStops = busStops.findClosestStops(location.first, location.second);
            
            buildingJson["nearbyStops"] = json::object();
            
            if (closestStops.first) {
                try {
                    json northbound = busStopToJson(*closestStops.first);
                    double distance = distBetween2Points(location.first, location.second, closestStops.first->Latitude, closestStops.first->Longitude);
                    northbound["distance"] = distance;
                    json predictionsResult = getBusPredictionsJson(closestStops.first->ID, closestStops.first->Route, curl, apiKey);
                    if (predictionsResult.find("predictions") != predictionsResult.end()) {
                        northbound["predictions"] = predictionsResult["predictions"];
                    } else {
                        northbound["predictions"] = json::array();
                    }
                    buildingJson["nearbyStops"]["northbound"] = northbound;
                } catch (const exception& e) {
                    // If predictions fail, still include the stop without predictions
                    json northbound = busStopToJson(*closestStops.first);
                    double distance = distBetween2Points(location.first, location.second, closestStops.first->Latitude, closestStops.first->Longitude);
                    northbound["distance"] = distance;
                    northbound["predictions"] = json::array();
                    buildingJson["nearbyStops"]["northbound"] = northbound;
                }
            }
            
            if (closestStops.second) {
                try {
                    json southbound = busStopToJson(*closestStops.second);
                    double distance = distBetween2Points(location.first, location.second, closestStops.second->Latitude, closestStops.second->Longitude);
                    southbound["distance"] = distance;
                    json predictionsResult = getBusPredictionsJson(closestStops.second->ID, closestStops.second->Route, curl, apiKey);
                    if (predictionsResult.find("predictions") != predictionsResult.end()) {
                        southbound["predictions"] = predictionsResult["predictions"];
                    } else {
                        southbound["predictions"] = json::array();
                    }
                    buildingJson["nearbyStops"]["southbound"] = southbound;
                } catch (const exception& e) {
                    // If predictions fail, still include the stop without predictions
                    json southbound = busStopToJson(*closestStops.second);
                    double distance = distBetween2Points(location.first, location.second, closestStops.second->Latitude, closestStops.second->Longitude);
                    southbound["distance"] = distance;
                    southbound["predictions"] = json::array();
                    buildingJson["nearbyStops"]["southbound"] = southbound;
                }
            }
            
            result.push_back(buildingJson);
        }
    }
    
    return result;
}

