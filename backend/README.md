# CTA Bus Predictor - Backend

C++ REST API server for the CTA Bus Predictor application.

## Structure

- **Source files**: All `.cpp` and `.h` files
- **Data files**: `nu.osm` (OpenStreetMap data), `bus-stops.txt` (bus stop data)
- **Dependencies**: `httplib.h` (HTTP server), `json.hpp` (JSON parsing)
- **Build output**: `server` (executable)

## Prerequisites

- C++ compiler with C++17 support (g++ or clang++)
- `libcurl` installed
  - macOS: `brew install curl`
  - Linux: `sudo apt install libcurl4-openssl-dev`
- `pthread` library (usually included with compiler)

## Build

```bash
make build-server
```

## Run

```bash
./server
# or
make run-server
# or use the helper script
./start-server.sh
```

The server will start on `http://localhost:8080` by default.

## API Endpoints

- `GET /api/health` - Health check and statistics
- `GET /api/buildings` - Get all buildings
- `GET /api/buildings/search?name=<name>` - Search buildings by name
- `GET /api/bus-stops` - Get all bus stops
- `GET /api/bus-stops/nearby?lat=<lat>&lon=<lon>` - Find nearby bus stops
- `GET /api/predictions?stopId=<id>&route=<route>` - Get bus predictions for a stop

All endpoints return JSON and support CORS for frontend access.

## Original CLI Application

The original command-line interface is still available:

```bash
make build
./a.out
```

