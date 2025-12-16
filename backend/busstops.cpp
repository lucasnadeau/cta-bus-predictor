/*busstops.cpp*/

#include <iostream>
#include <fstream>
#include <sstream>
#include "busstops.h"

using namespace std;

//
// Constructor
//
BusStops::BusStops(const string& filename)
{
  ifstream infile(filename);

  if (!infile.is_open()) {
    cerr << "**ERROR: Unable to open bus stops file: " << filename << endl;
    return;
  }

  string line;
  while (getline(infile, line)) {
    stringstream ss(line);

    string idStr, route, stopName, direction, location, latStr, lonStr;

    getline(ss, idStr, ',');
    getline(ss, route, ',');
    getline(ss, stopName, ',');
    getline(ss, direction, ',');
    getline(ss, location, ',');
    getline(ss, latStr, ',');
    getline(ss, lonStr);

    long long id = stoll(idStr);
    double latitude = stod(latStr);
    double longitude = stod(lonStr);

    BusStop stop(id, route, stopName, direction, location, latitude, longitude);
    StopList.push_back(stop);
  }

  infile.close();
}

//
// Prints all bus stops
//
void BusStops::print()
{
  // Sort bus stops by ID
  sort(StopList.begin(), StopList.end(), [](const BusStop& a, const BusStop& b) {
    return a.ID < b.ID;
  });

  // Print each stop
  for (const BusStop& stop : StopList) {
    stop.print();
  }
}


pair<const BusStop*, const BusStop*> BusStops::findClosestStops(double lat, double lon) const {
    const BusStop* closestNorthbound = nullptr;
    const BusStop* closestSouthbound = nullptr;
    double minDistanceNorthbound = numeric_limits<double>::max();
    double minDistanceSouthbound = numeric_limits<double>::max();

    for (const BusStop& stop : StopList) {
        double distance = distBetween2Points(lat, lon, stop.Latitude, stop.Longitude);
      
        if (stop.Direction == "Northbound" && distance < minDistanceNorthbound) {
            closestNorthbound = &stop;
            minDistanceNorthbound = distance;
        } else if (stop.Direction == "Southbound" && distance < minDistanceSouthbound) {
            closestSouthbound = &stop;
            minDistanceSouthbound = distance;
        }
    }

    return make_pair(closestNorthbound, closestSouthbound);
}