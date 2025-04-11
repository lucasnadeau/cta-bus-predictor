/*busstop.h*/


#pragma once

#include <string>
#include <algorithm>
#include "curl_util.h"

using namespace std;

class BusStop
{
public:
  long long ID;
  string Route;
  string StopName;
  string Direction;
  string Location;
  double Latitude;
  double Longitude;

  //
  // Constructor
  //
  BusStop(long long id, string route, string stopName, string direction, string location, double latitude, double longitude);

  //
  // Print
  //
  // Prints bus stop details
  //
  void print() const;

  void printBusPredictions(CURL* curl, const string& apiKey) const;
};