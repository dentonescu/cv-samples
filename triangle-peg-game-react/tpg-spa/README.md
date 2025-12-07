# Triangle Peg Game SPA
React + Vite single-page app for the triangular peg solitaire puzzle. The UI renders the 15-hole triangle, highlights legal jumps, animates moves, and exposes undo/reset plus status messaging.

## Quick start
```bash
npm install
npm run dev    # start Vite dev server
npm run build  # production build
```

## Tech stack
- React with hooks and functional components
- Vite tooling
- CSS for layout/animation; Bootstrap utility classes may be pulled in for quick spacing if needed
- Bitmask-based game logic; optional Python helper to precompute board states

## Project status
- UI prototype exists in [UI prototype](../prototype/triange_peg_game_ui_prototype.html).
- React scaffold is live; next steps are wiring board rendering, move validation, and history/undo.
