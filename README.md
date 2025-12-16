# CTA Bus Stop Predictor
## By Lucas Nadeau

This C++ project parses OpenStreetMap (OSM) data to locate Chicago Transit Authority (CTA) bus stops near a user-specified location, and fetches live arrival predictions using the CTA Bus Tracker API.

## Features

- Parses `.osm` XML data using TinyXML2
- Identifies and filters CTA bus stops from OSM data
- Computes the nearest bus stop based on geographic coordinates
- Uses `libcurl` to make real-time API calls to CTA Bus Tracker
- Displays upcoming bus arrival times for the nearest stop

## Technologies Used

- **C++17**
- **libcurl** for HTTP requests
- **TinyXML2** for parsing OSM files
- **CTA Bus Tracker API**: [bustime API](http://www.ctabustracker.com/bustime/)


## Build & Run

### Prerequisites
- C++ compiler with C++17 support
- `libcurl` installed (`sudo apt install libcurl4-openssl-dev`)
- XML parser library (`tinyxml2`, if used)

### Compile the project

make

## Run the project

./cta-bus-predictor data/sample_chicago.osm

You’ll be prompted to enter a latitude and longitude. The program will:

1. Parse the provided .osm file
2. Find the nearest CTA bus stop
3. Call the CTA Bus Tracker API using the stop ID
4. Display real-time predictions for the next arriving buses at that stop