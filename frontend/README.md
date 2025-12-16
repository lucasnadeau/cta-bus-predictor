# CTA Bus Predictor - Frontend

Next.js web application for searching buildings and viewing CTA bus arrival predictions.

## Structure

- **app/**: Next.js App Router pages and components
- **lib/**: API client and utilities
- **Configuration**: `next.config.js`, `tsconfig.json`, `package.json`

## Prerequisites

- Node.js 18+ and npm

## Setup

```bash
npm install
```

## Development

```bash
npm run dev
```

The frontend will be available at `http://localhost:3000`.

## Build

```bash
npm run build
```

## Production

```bash
npm start
```

## Configuration

The frontend is configured to proxy API requests to the backend server at `http://localhost:8080` via Next.js rewrites (see `next.config.js`).

To change the backend URL, update `next.config.js` or set the `NEXT_PUBLIC_API_URL` environment variable.

