/*busstop.cpp*/

#include <iostream>
#include "busstop.h"
#include "json.hpp"
#include "curl_util.h"
#include <stdexcept>


using namespace std;
using json = nlohmann::json;

//
// Constructor
//
BusStop::BusStop(long long id, string route, string stopName, string direction, string location, double latitude, double longitude)
  : ID(id), Route(route), StopName(stopName), Direction(direction), Location(location), Latitude(latitude), Longitude(longitude)
{
}

//
// Print
//
// Prints bus stop details
//
void BusStop::print() const
{
  cout << ID << ": bus " << Route << ", " << StopName << ", " << Direction << ", " << Location << ", location (" << Latitude << ", " << Longitude << ")" << endl;
}

//
// printBusPredictions
//
// Fetches bus arrival predictions from the CTA API for this stop and prints them.
//
void BusStop::printBusPredictions(CURL* curl, const string& apiKey) const {
    string url = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=" + apiKey +
                 "&rt=" + Route + "&stpid=" + to_string(ID) + "&format=json";

    string response;
    bool success = callWebServer(curl, url, response);

    if (!success) {
        cout << "  <<bus predictions unavailable, call failed>>" << endl;
        return;
    }

    auto jsondata = json::parse(response);
    auto busResponse = jsondata["bustime-response"];

    if (busResponse.find("prd") == busResponse.end()) {

        cout << "  <<no predictions available>>" << endl;
        return;
    }

    auto predictions = busResponse["prd"];
    for (auto& prediction : predictions) {
        try {
            // Extract and print prediction details
            int vehicleId = stoi(prediction["vid"].get_ref<std::string&>());
            string route = prediction["rt"].get<std::string>();
            string direction = prediction["rtdir"].get<std::string>();
            string time = prediction["prdctdn"].get<std::string>();

            // Handle special cases for the time
            if (time == "DUE") {
                time = "due";
            } else if (time.empty()) {
                time = "unknown";
            }

cout << " vehicle #" << stoi(prediction["vid"].get_ref<std::string&>()) << " on route " << prediction["rt"].get<std::string>() << " travelling " << prediction["rtdir"].get<std::string>() << " to arrive in " << prediction["prdctdn"].get<std::string>() << " mins" << endl;
        }
        catch (const exception& e) {

            cout << " error" << endl;
            cout << "  malformed CTA response, prediction unavailable"
                 << " (error: " << e.what() << ")" << endl;
        }
    }
}
