/*curl_util.h*/

//
// CURL utility functions for calling a web server.
// 


#pragma once

#include <iostream>
#include <string>

#include <curl/curl.h>

using namespace std;

//
// callWebServer:
//
//
bool callWebServer(CURL* curl, string url, string& response);