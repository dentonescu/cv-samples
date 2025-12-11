# Triangle Peg Game SPA
React + Vite single-page app for the triangular peg solitaire puzzle. The UI renders the 15-hole triangle, highlights legal jumps, animates moves, and surfaces win/lose messaging (undo/reset queued next).

Live preview: [triangle-peg-game-react.vercel.app](https://triangle-peg-game-react.vercel.app)

## Quick start
```bash
npm install
npm run dev    # start Vite dev server
npm run build  # production build
npm test       # sanity check precomputed states
```

## Tech stack
- React with hooks and functional components
- Vite tooling
- CSS for layout/animation
- Bitmask-based game logic; Python precompute helper emits `public/states.json` for quick lookups

## Project status
- UI prototype exists in [UI prototype](../prototype/triange_peg_game_ui_prototype.html).
- React components render from the bitmask and precomputed states; legal targets glow; end-of-game overlay shows win/lose.
- Precompute utility generated `public/states.json` (full 15-bit state space).
- Tests: `npm test` runs a Node sanity check against the precomputed states.
- Next steps: add undo/reset controls, broaden test coverage, and polish animations.
