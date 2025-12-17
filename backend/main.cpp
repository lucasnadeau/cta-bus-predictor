/*main.cpp*/

//
// Program to input Nodes (positions) and Buildings from
// an Open Street Map file.
// 

#include <iostream>
#include <string>

#include "building.h"
#include "buildings.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "busstop.h"
#include "busstops.h"
#include "curl_util.h"

using namespace std;
using namespace tinyxml2;


//
// main
//
int main()
{
  XMLDocument xmldoc;
  Nodes nodes;
  Buildings buildings;

  string apiKey = "ENTER YOUR API KEY HERE";


  

  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
  cout << "**ERROR:" << endl;
  cout << "**ERROR: unable to initialize curl library" << endl;
  cout << "**ERROR:" << endl;
  return 0;
  }
  
  cout << "** NU open street map **" << endl;

  string filename;

  cout << endl;
  cout << "Enter map filename> " << endl;
  getline(cin, filename);

  //
  // 1. load XML-based map file 
  //
  if (!osmLoadMapFile(filename, xmldoc))
  {
    // failed, error message already output
    cout << "**ERROR:" << endl;
    cout << "**ERROR: call to osmLoadMapFile failed, perhaps filename '" << filename << "' does not exist?" << endl;
    cout << "**ERROR:" << endl;
    return 0;
  }
  
  //
  // 2. read the nodes, which are the various known positions on the map:
  //
  nodes.readMapNodes(xmldoc);

  //
  // 3. read the university buildings:
  //
  buildings.readMapBuildings(xmldoc);

  //
  // 4. stats
  //
  cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
  cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;


  BusStops busStops("bus-stops.txt");

  cout << "# of bus stops: " << busStops.StopList.size() << endl;

  // Optional: Test printing all bus stops
  // busStops.print();


  //
  // now let the user for search for 1 or more buildings:
  //
  while (true)
  {
    string name;

    cout << endl;
    cout << "Enter building name (partial or complete), or * to list, or @ for bus stops, or $ to end> " << endl;

    getline(cin, name);

    if (name == "$") {
      break;
    }
    else if (name == "*") {
      buildings.print();
    }
    else if (name == "@"){
      busStops.print();
    }
    else {
      buildings.findAndPrint(name, nodes, busStops, curl, apiKey);


    }//else  

  }//while

  //
  // done:
  //
  curl_easy_cleanup(curl);
  cout << endl;
  cout << "** Done  **" << endl;

  // cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
  // cout << "# of Nodes created: " << Node::getCreated() << endl;
  // cout << "# of Nodes copied: " << Node::getCopied() << endl;
  cout << endl;

  return 0;
}