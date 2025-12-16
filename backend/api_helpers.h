/*api_helpers.h*/

#pragma once

#include <string>
#include "buildings.h"
#include "busstops.h"
#include "nodes.h"
#include "curl_util.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Helper functions to return JSON instead of printing

json buildingToJson(const Building& building, const Nodes& nodes);
json buildingsToJson(const Buildings& buildings, const Nodes& nodes);
json searchBuildingsJson(const string& name, const Buildings& buildings, const Nodes& nodes, const BusStops& busStops, CURL* curl, const string& apiKey);
json busStopToJson(const BusStop& stop);
json busStopsToJson(const BusStops& busStops);
json getBusPredictionsJson(long long stopId, const string& route, CURL* curl, const string& apiKey);
json nearbyStopsJson(double lat, double lon, const BusStops& busStops, CURL* curl, const string& apiKey);

