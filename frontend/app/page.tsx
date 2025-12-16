'use client'

import { useState } from 'react'
import { searchBuildings, Building, BusStopWithPredictions } from '@/lib/api'
import './globals.css'

export default function Home() {
  const [searchTerm, setSearchTerm] = useState('')
  const [buildings, setBuildings] = useState<Building[]>([])
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  const handleSearch = async () => {
    if (!searchTerm.trim()) {
      setError('Please enter a building name')
      return
    }

    setLoading(true)
    setError(null)

    try {
      const results = await searchBuildings(searchTerm)
      setBuildings(results)
      if (results.length === 0) {
        setError('No buildings found matching your search')
      }
    } catch (err) {
      setError('Failed to search buildings. Make sure the backend server is running on http://localhost:8080')
      console.error(err)
    } finally {
      setLoading(false)
    }
  }

  const handleKeyPress = (e: React.KeyboardEvent) => {
    if (e.key === 'Enter') {
      handleSearch()
    }
  }

  return (
    <div>
      <header className="header">
        <div className="container">
          <h1>CTA Bus Stop Predictor</h1>
          <p style={{ textAlign: 'center', marginTop: '0.75rem', fontSize: '1.125rem', color: '#6b7280', fontWeight: '400' }}>
            Find nearby bus stops and real-time arrival predictions
          </p>
        </div>
      </header>

      <main className="container">
        <div className="search-section card">
          <h2>Find Your Building</h2>
          <p style={{ color: '#6b7280', fontSize: '0.9375rem', marginBottom: '1.5rem', marginTop: '-0.5rem' }}>
            Search by building name to discover nearby bus stops and arrival times
          </p>
          <div style={{ display: 'flex', gap: '1rem', flexWrap: 'wrap' }}>
            <input
              type="text"
              className="search-input"
              placeholder="Enter building name (e.g., Mudd, Tech)"
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              onKeyPress={handleKeyPress}
            />
            <button
              className="button"
              onClick={handleSearch}
              disabled={loading}
            >
              {loading ? 'Searching...' : 'Search'}
            </button>
          </div>
        </div>

        {error && (
          <div className="error">{error}</div>
        )}

        {loading && (
          <div className="loading">Loading</div>
        )}

        {!loading && buildings.length > 0 && (
          <div className="building-list">
            {buildings.map((building) => (
              <BuildingCard key={building.id} building={building} />
            ))}
          </div>
        )}

        {!loading && buildings.length === 0 && !error && (
          <div className="empty-state">
            <p>Search for a building to see nearby bus stops and arrival predictions</p>
          </div>
        )}
      </main>
    </div>
  )
}

function BuildingCard({ building }: { building: Building }) {
  return (
    <div className="card building-item">
      <div className="building-name">{building.name}</div>
      <div className="building-address">{building.streetAddress}</div>
      <div className="building-location">
        {building.location.latitude.toFixed(6)}, {building.location.longitude.toFixed(6)}
      </div>

      {building.nearbyStops && (
        <div className="stops-section">
          <h3>Nearby Bus Stops</h3>
          
          {building.nearbyStops.northbound && (
            <StopCard stop={building.nearbyStops.northbound} direction="Northbound" />
          )}
          
          {building.nearbyStops.southbound && (
            <StopCard stop={building.nearbyStops.southbound} direction="Southbound" />
          )}

          {!building.nearbyStops.northbound && !building.nearbyStops.southbound && (
            <div className="empty-state" style={{ padding: '1rem', background: 'transparent', border: 'none' }}>
              No nearby bus stops found
            </div>
          )}
        </div>
      )}
    </div>
  )
}

function StopCard({ stop, direction }: { stop: BusStopWithPredictions; direction: string }) {
  return (
    <div className="stop-item">
      <div className="stop-header">
        <div className="stop-name">{stop.stopName} - {direction}</div>
        {stop.distance !== undefined && (
          <div className="stop-distance">{stop.distance.toFixed(2)} mi</div>
        )}
      </div>
      <div className="stop-details">
        <span>Route: {stop.route}</span>
        <span>Stop ID: {stop.id}</span>
        <span>{stop.location}</span>
      </div>

      {stop.predictions && stop.predictions.length > 0 ? (
        <div className="predictions-list">
          <div>Arrival Predictions:</div>
          {stop.predictions.map((prediction, index) => (
            <div key={index} className="prediction-item">
              <div>
                <span className="prediction-time">
                  {prediction.arrivalTime === 'due' ? 'Due now' : 
                   prediction.arrivalTime === 'unknown' ? 'Time unknown' :
                   `${prediction.arrivalTime} min${prediction.arrivalTime !== '1' ? 's' : ''}`}
                </span>
                {' • '}
                Vehicle #{prediction.vehicleId} on route {prediction.route} 
                {' '}({prediction.direction})
              </div>
            </div>
          ))}
        </div>
      ) : (
        <div style={{ color: '#999', fontStyle: 'italic', marginTop: '0.5rem', padding: '0.5rem' }}>
          No predictions available
        </div>
      )}
    </div>
  )
}

