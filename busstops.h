/*busstops.h*/


#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "busstop.h"
#include "dist.h"
#include <cmath>

using namespace std;

class BusStops
{
public:
  vector<BusStop> StopList;

  //
  // Constructor
  //
  BusStops(const string& filename);

  //
  // Print
  //
  // Prints all bus stops
  //
  void print();

    //     
    // findClosestStops
    //
    // returns closest stops to a given location, returns a pair
    //
    pair<const BusStop*, const BusStop*> findClosestStops(double lat, double lon) const;
};