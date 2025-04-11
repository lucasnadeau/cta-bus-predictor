/*osm.h*/

//
// Functions for working with an Open Street Map file.
//

#pragma once

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// Helper functions:
//
bool osmLoadMapFile(string filename, XMLDocument& xmldoc);
bool osmContainsKeyValue(XMLElement* e, string key, string value);
string osmGetKeyValue(XMLElement* e, string key);