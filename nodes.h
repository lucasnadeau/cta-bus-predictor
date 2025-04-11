/*nodes.h*/

//
// A collection of nodes in the Open Street Map.
//

#pragma once

#include <vector>

#include "node.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// Keeps track of all the nodes in the map.
//
class Nodes
{
private:
  vector<Node> MapNodes;

public:
  //
  // readMapNodes
  //
  // Given an XML document, reads through the document and 
  // stores all the nodes into the given vector. Each node
  // is a point on the map, with a unique id along with 
  // (lat, lon) position. Some nodes are entrances to buildings,
  // which we capture as well.
  //
  void readMapNodes(XMLDocument& xmldoc);

  //
  // sortByID
  //
  // sorts the nodes into ascending order by ID.
  //
  void sortByID();

  //
  // find
  // 
  // Searches the nodes for the one with the matching ID, returning
  // true if found and false if not. If found, the node's Lat, Lon,
  // and IsEntrance data are returned via the reference parameters.
  //
  bool find(long long id, double& lat, double& lon, bool& isEntrance) const;

  //
  // accessors / getters
  //
  int getNumMapNodes();

};
