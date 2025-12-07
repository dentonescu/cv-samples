# Developer notes

Current status: UI prototype is tuned; Vite/React scaffold is up. Next focus is wiring game logic, rendering, and controls for a deployable Vercel build.

## Repository layout
```
triangle-peg-game-react/
├─ README.md              # project overview + rules
├─ NOTES.md               # you are here
├─ prototype/             # static HTML/CSS layout prototype
│  └─ triange_peg_game_ui_prototype.html
├─ img/                   # reference photos + prototype snapshot
└─ tpg-spa/               # Vite React app
   ├─ src/App.jsx         # to be replaced with board/controls components
   ├─ public/             # static assets
   └─ ...                 # Vite config, npm metadata
```

## Initial project setup
```bash
npm create vite@latest tpg-spa -- --template react
cd tpg-spa
npm install
npm run dev
```

## Game representation
- 15 holes → use a 15-bit mask (`1` = peg present).
- Hole indexing: top to bottom, left to right within each row (0..14). Lock this into a shared constant.
- Legal moves: static list of `{ from, over, to }` triples that conform to the triangle adjacency; reuse for validation and highlighting.
- Apply move: clear `from` and `over`, set `to`; push prior mask to history for undo.
- Optional: Python script to precompute the reachable state set and emit a JS module if needed; on-the-fly validation from the static move list is already cheap.

## Planned components (tpg-spa/src)
- `Board` renders the triangular grid, accepts `boardMask`, `selected`, `legalTargets`, and click handlers.
- `Hole` renders a single position (peg or empty) and highlights when selectable/targetable.
- `Controls` hosts reset/undo and any “show moves” toggle.
- `StatusBar` shows peg count, move availability, and solved/no-move messages.
- Logic helpers in `logic/` (e.g., `moves.js`, `boardUtils.js`) stay pure and do not touch React state directly.

## Prototype reference
- [UI prototype](prototype/triange_peg_game_ui_prototype.html) captures the sizing, spacing, and clip-path for the board/peg layout. The React components should mirror these proportions.
- Snapshot: [Prototype snapshot](img/tpg--ui-prototype.png).

## Implementation plan (current focus)
1) Bitmask utilities + static move list (JS module).
2) Board/Hole components that render from the mask and surface click events.
3) Move application, selection + legal target highlighting, peg count, and history/undo.
4) Animations and responsive polish; match the prototype spacing.
5) README/docs refresh and Vercel-ready build check.

## Deployment target
- Vercel static hosting using `npm run build` output from Vite. Keep assets relative and avoid server-only APIs.
