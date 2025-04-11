/*buildings.h*/

//
// A collection of buildings in the Open Street Map.
// 

#pragma once

#include <vector>
#include <iostream>

#include "building.h"
#include "tinyxml2.h"
#include "busstops.h"


using namespace std;
using namespace tinyxml2;


//
// Keeps track of all the buildings in the map.
//
class Buildings
{
public:
  vector<Building> MapBuildings;

  //
  // readMapBuildings
  //
  // Given an XML document, reads through the document and 
  // stores all the buildings into the given vector.
  //
  void readMapBuildings(XMLDocument& xmldoc);

  //
  // accessors / getters
  //
  int getNumMapBuildings();
     
  //
  // print
  // 
  //prints all the buildings, one per line
  //
  void print();


  //
  // findAndPrint
  //
  // searches and prints over the buildings class
  //
void findAndPrint(const string& name, const Nodes& nodes, const BusStops& busStops);
};

