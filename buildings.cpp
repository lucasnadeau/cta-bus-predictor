/*buildings.cpp*/

//
// A collection of buildings in the Open Street Map.
// 

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "buildings.h"
#include "osm.h"
#include "tinyxml2.h"
#include "curl_util.h"

using namespace std;
using namespace tinyxml2;


//
// readMapBuildings
//
// Given an XML document, reads through the document and 
// stores all the buildings into the given vector.
//
void Buildings::readMapBuildings(XMLDocument& xmldoc)
{
  XMLElement* osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document way by way, looking for university buildings:
  //
  XMLElement* way = osm->FirstChildElement("way");

  while (way != nullptr)
  {
    const XMLAttribute* attr = way->FindAttribute("id");
    assert(attr != nullptr);

    //
    // if this is a building, store info into vector:
    //
    if (osmContainsKeyValue(way, "building", "university"))
    {
      string name = osmGetKeyValue(way, "name");

      string streetAddr = osmGetKeyValue(way, "addr:housenumber")
        + " "
        + osmGetKeyValue(way, "addr:street");

      //
      // create building object, then add the associated
      // node ids to the object:
      //
      long long id = attr->Int64Value();

      Building B(id, name, streetAddr);

      XMLElement* nd = way->FirstChildElement("nd");

      while (nd != nullptr)
      {
        const XMLAttribute* ndref = nd->FindAttribute("ref");
        assert(ndref != nullptr);

        long long id = ndref->Int64Value();

        B.add(id);

        // advance to next node ref:
        nd = nd->NextSiblingElement("nd");
      }

      //
      // add the building to the vector:
      //
      this->MapBuildings.push_back(B);
    }//if

    way = way->NextSiblingElement("way");
  }//while

  //
  // done:
  //
}

//
// accessors / getters
//
int Buildings::getNumMapBuildings() {
  return (int) this->MapBuildings.size();
}

//
// print
//
// Prints all buildings, one per line
//
void Buildings::print() {
  for (const Building& B : this->MapBuildings) {
    cout << B.ID << ": " << B.Name << ", " << B.StreetAddress << endl;
  }
}

  //
  // findAndPrint
  //
  // searches and prints over the buildings class
  //
void Buildings::findAndPrint(const string& name, const Nodes& nodes, const BusStops& busStops, CURL* curl, const string& apiKey)
{
  bool foundAtLeastOne = false;
 // const Building* building = nullptr;
  for (Building& B : this->MapBuildings)
  {
    if (B.Name.find(name) != string::npos)
    {
      foundAtLeastOne = true;
      B.print(nodes);
     // building = &B;    
      auto location = B.getLocation(nodes);
 //     cout << "Building location: (" << location.first << ", " << location.second << ")" << endl;

      auto closestStops = busStops.findClosestStops(location.first, location.second);

                if (closestStops.second) {
                  double distanceSouthbound = distBetween2Points(
                    location.first, location.second,
                    closestStops.second->Latitude, closestStops.second->Longitude);

                    cout << "Closest southbound stop:" << endl;
 
                    cout << " " << closestStops.second->ID << ": " << closestStops.second->StopName << ", " << "bus #" << closestStops.second->Route << ", " << closestStops.second->Location << ", "<< distanceSouthbound << " miles" <<endl;
                    closestStops.second->printBusPredictions(curl, apiKey);
                } else {
                    cout << "No southbound stop found" << endl;
                }

                if (closestStops.first) {
                  double distanceNorthbound = distBetween2Points(
                    location.first, location.second,
                    closestStops.first->Latitude, closestStops.first->Longitude);
                    cout << "Closest northbound stop:" << endl;

                    cout << " " << closestStops.first->ID << ": " << closestStops.first->StopName << ", " << "bus #" << closestStops.first->Route << ", " << closestStops.first->Location << ", "<< distanceNorthbound << " miles" <<endl;
                    closestStops.first->printBusPredictions(curl, apiKey);
                } else {
                    cout << "No northbound stop found." << endl;
                }
            }


    }



  if (!foundAtLeastOne) {
    cout << "No such building" << endl;
}
}