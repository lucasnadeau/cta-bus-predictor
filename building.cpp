/*building.cpp*/

//
// A building in the Open Street Map.
// 
// 

#include "building.h"


using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}


  //
  // print
  //
  // Prints the building's information, including its nodes, using the given Nodes object
  //
void Building::print(const Nodes& nodes)
{
  cout << Name << endl;
  cout << "Address: " << StreetAddress << endl;
  cout << "Building ID: " << ID << endl;
  cout << "# perimters nodes: " << NodeIDs.size() << endl;

  // prints the building's location
  pair<double, double> P = getLocation(nodes);
  cout << "Location: (" << P.first << ", " << P.second << ")" << endl;

  // cout << "Nodes:" << endl;
  // for (long long nodeid : NodeIDs)
  // {
  //   cout << "  " << nodeid << ": ";

  //   double lat = 0.0;
  //   double lon = 0.0;
  //   bool entrance = false;

  //   bool found = nodes.find(nodeid, lat, lon, entrance);
    
  //   if (found) {
  //     cout << "(" << lat << ", " << lon << ")";

  //     if (entrance)
  //       cout << ", is entrance";

  //     cout << endl;
  //   }
  //   else {
  //     cout << "**NOT FOUND**" << endl;
  //   }
  // }
}

//
// gets the center (lat, lon) of the building based
// on the nodes that form the perimeter
//
pair<double, double> Building::getLocation(const Nodes& nodes)
{
  int count = 0;
  double sumLat = 0.0;
  double sumLon = 0.0;
  for (long long nodeid : NodeIDs)
  {

    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    bool found = nodes.find(nodeid, lat, lon, entrance);
    
    if (found) {
      sumLat += lat;
      sumLon += lon;
      count++;
      }
  }

  if (count == 0) {
    return make_pair(0.0, 0.0);
  }

  return make_pair(sumLat / count, sumLon / count);


}

