#!/bin/bash
# Start the C++ backend server

cd "$(dirname "$0")"

# Kill any existing server processes
lsof -ti:8080 | xargs kill -9 2>/dev/null

# Build if needed
if [ ! -f ./server ]; then
    echo "Building server..."
    make build-server
fi

# Start the server
echo "Starting server on http://localhost:8080..."
echo "Data files: nu.osm, bus-stops.txt"
./server

