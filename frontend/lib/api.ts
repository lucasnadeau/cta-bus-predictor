// API client for C++ backend
// Use Next.js proxy in browser, direct connection in server-side

const API_BASE_URL = typeof window === 'undefined' 
  ? (process.env.NEXT_PUBLIC_API_URL || 'http://localhost:8080') + '/api'
  : '/backend-api'; // Use Next.js proxy (which rewrites to http://localhost:8080/api)

export interface Building {
  id: number;
  name: string;
  streetAddress: string;
  location: {
    latitude: number;
    longitude: number;
  };
  nearbyStops?: {
    northbound?: BusStopWithPredictions;
    southbound?: BusStopWithPredictions;
  };
}

export interface BusStop {
  id: number;
  route: string;
  stopName: string;
  direction: string;
  location: string;
  latitude: number;
  longitude: number;
  distance?: number;
}

export interface BusStopWithPredictions extends BusStop {
  predictions: BusPrediction[];
}

export interface BusPrediction {
  vehicleId: number;
  route: string;
  direction: string;
  arrivalTime: string;
}

export interface NearbyStopsResponse {
  location: {
    latitude: number;
    longitude: number;
  };
  stops: {
    northbound?: BusStopWithPredictions;
    southbound?: BusStopWithPredictions;
  };
}

export async function getBuildings(): Promise<Building[]> {
  const response = await fetch(`${API_BASE_URL}/buildings`);
  if (!response.ok) {
    throw new Error('Failed to fetch buildings');
  }
  return response.json();
}

export async function searchBuildings(name: string): Promise<Building[]> {
  const response = await fetch(`${API_BASE_URL}/buildings/search?name=${encodeURIComponent(name)}`);
  if (!response.ok) {
    throw new Error('Failed to search buildings');
  }
  return response.json();
}

export async function getBusStops(): Promise<BusStop[]> {
  const response = await fetch(`${API_BASE_URL}/bus-stops`);
  if (!response.ok) {
    throw new Error('Failed to fetch bus stops');
  }
  return response.json();
}

export async function getNearbyStops(lat: number, lon: number): Promise<NearbyStopsResponse> {
  const response = await fetch(`${API_BASE_URL}/bus-stops/nearby?lat=${lat}&lon=${lon}`);
  if (!response.ok) {
    throw new Error('Failed to fetch nearby stops');
  }
  return response.json();
}

export async function getPredictions(stopId: number, route: string): Promise<{ predictions: BusPrediction[] }> {
  const response = await fetch(`${API_BASE_URL}/predictions?stopId=${stopId}&route=${encodeURIComponent(route)}`);
  if (!response.ok) {
    throw new Error('Failed to fetch predictions');
  }
  return response.json();
}

