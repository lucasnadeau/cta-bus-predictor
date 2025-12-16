# CTA Bus Stop Predictor
## By Lucas Nadeau

This project consists of a C++ REST API backend and a Next.js web frontend that helps locate Chicago Transit Authority (CTA) bus stops near buildings and displays real-time bus arrival predictions.

## Features

- Parses `.osm` XML data using TinyXML2
- Identifies and filters CTA bus stops from OSM data
- Computes the nearest bus stop based on building you are near
- Uses `libcurl` to make real-time API calls to CTA Bus Tracker
- Displays upcoming bus arrival times for the nearest stop

## Technologies Used

### Backend
- **C++17**
- **cpp-httplib** for HTTP server
- **libcurl** for HTTP requests
- **TinyXML2** for parsing OSM files
- **nlohmann/json** for JSON handling
- **CTA Bus Tracker API**: [bustime API](http://www.ctabustracker.com/bustime/)

### Frontend
- **Next.js 14** with App Router
- **React 18**
- **TypeScript**

## Prerequisites

### Backend
- C++ compiler with C++17 support (g++ or clang++)
- `libcurl` installed
  - macOS: `brew install curl`
  - Linux: `sudo apt install libcurl4-openssl-dev`
- `pthread`

### Frontend
- Node.js 18+ and npm

## Build & Compile

### Backend Server

```bash
cd backend
make build-server
```

This compiles the HTTP server executable (`server`).

### Original CLI Application

```bash
cd backend
make build
```

This compiles the original command-line interface (`a.out`).

### Frontend

```bash
cd frontend
npm install
```

This installs all Node.js dependencies.

## Run

### Option 1: Web Application (Recommended)

1. **Start the backend server:**
   ```bash
   cd backend
   make run-server
   # or
   ./server
   # or use the helper script
   ./start-server.sh
   ```
   
   The server will start on `http://localhost:8080`

2. **Start the frontend (in a new terminal):**
   ```bash
   cd frontend
   npm run dev
   ```
   
   The frontend will be available at `http://localhost:3000`

3. **Open your browser:**
   Navigate to `http://localhost:3000` and search for buildings!

### Option 2: Command-Line Interface

```bash
cd backend
make build
./a.out
```

You'll be prompted to enter a building name. The program will:
1. Parse the provided .osm file
2. Find matching buildings
3. Find the nearest CTA bus stops (northbound and southbound)
4. Display real-time predictions for the next arriving buses

## Usage

### Web Application

1. Open `http://localhost:3000` in your browser
2. Enter a building name in the search box (e.g., "Tech", "Mudd")
3. Click "Search" or press Enter
4. View the results showing:
   - Building information (name, address, location)
   - Nearby bus stops (northbound and southbound)
   - Real-time bus arrival predictions for each stop

### API Endpoints

The backend exposes the following REST API endpoints:

- `GET /api/health` - Health check and statistics
- `GET /api/buildings` - Get all buildings
- `GET /api/buildings/search?name=<name>` - Search buildings by name
- `GET /api/bus-stops` - Get all bus stops
- `GET /api/bus-stops/nearby?lat=<lat>&lon=<lon>` - Find nearby bus stops
- `GET /api/predictions?stopId=<id>&route=<route>` - Get bus predictions for a stop

All endpoints return JSON and support CORS for frontend access.

## Development

### Backend Development
```bash
cd backend
make build-server 
make run-server    
make build         
make clean    
```

### Frontend Development
```bash
cd frontend
npm run dev
npm run build 
npm start
```

## Configuration

The backend server loads data from:
- `nu.osm` - OpenStreetMap data (default)
- `bus-stops.txt` - Bus stop data (default)

You can specify different files when running the server:
```bash
./server [osm-file] [bus-stops-file]
```

The frontend is configured to proxy API requests to `http://localhost:8080` via Next.js rewrites. To change the backend URL, update `frontend/next.config.js` or set the `NEXT_PUBLIC_API_URL` environment variable.