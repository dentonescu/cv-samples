# Frontend (llm-spa)

React + TypeScript frontend for the pwa‑llm‑poc project.

## Local development
```bash
npm install
npm run dev
```

The Vite dev server proxies `/api/*` calls to the Flask backend. See `vite.config.ts`.

## Build
```bash
npm run build
```

Build output is written to `dist/` and served by the production Docker image.

## PWA assets
- Manifest: `public/manifest.json`
- Service worker: `public/sw.js`
- Icons: `public/icons/`

## Navigation
- Back to the frontend overview: [../README.md](../README.md)
- Back to the project overview: [../../README.md](../../README.md)
