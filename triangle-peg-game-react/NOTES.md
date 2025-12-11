# Developer notes

Current status: Core play loop works. Board/Hole/Peg/Position/StatusBar are wired with bitmask logic, precomputed `public/states.json`, legal-move highlights, and end-of-game/win overlay. Python precompute still emits the 15-bit state space; lazy loader pulls it on app load. Live preview auto-deploys to [triangle-peg-game-react.vercel.app](https://triangle-peg-game-react.vercel.app) (work in progress). Next focus: undo/reset controls, richer tests, and animation polish.

## Repository layout
```
triangle-peg-game-react/
├─ README.md              # project overview + rules
├─ NOTES.md               # you are here
├─ precompute/            # Python helper that generates states.json
│  └─ precompute.py
├─ prototype/             # static HTML/CSS layout prototype
│  ├─ triange_peg_game_ui_prototype.html
│  └─ README.md
├─ img/                   # reference photos + prototype snapshot
└─ tpg-spa/               # Vite React app
   ├─ src/App.jsx         # root component
   ├─ src/components/     # Board/Hole/Peg/Position/StatusBar scaffolding
   ├─ src/logic/          # game helpers + lazy states loader
   ├─ public/             # static assets + precomputed states.json
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

## Coordinate mapping (1-based, closed-form)
Total holes `N` follow the triangle number formula `N = R(R+1)/2`, so for the 15-hole board `R=5` rows.

- Row from position:  
  $$r(N,n) = \left\lceil \frac{\sqrt{8n+1}-1}{2} \right\rceil$$
- Column from position (given `r` above):  
  $$c(N,n) = n - \frac{(r-1)r}{2}$$

Examples with `N=15`: `r(15,5)=3`, `c(15,5)=2`; `r(15,15)=5`, `c(15,15)=5`.

Reverse mapping (position from row/column):
$$p(N,r,c) = \frac{(r-1)r}{2} + c$$
For `N=15`, `p(15,3,2)=5`.

See: [Triangular number (Wikipedia)](https://en.wikipedia.org/wiki/Triangular_number)

Position to (row, column) in JavaScript:

```js
function rowFromPos(n) {
  return Math.ceil( (Math.sqrt(8*n+1) - 1) / 2);
}

function colFromPos(n) {
  return n - ((rowFromPos(n) - 1) * rowFromPos(n)) / 2;
}
```

Sanity check:

```js
for (let p = 1; p <= 15; ++p) {
  let r = rowFromPos(p);
  let c = colFromPos(p);
  console.log(`position: ${p}, row: ${r}, column: ${c}`);
}
```

![position to row and column](img/math--p2rc.png)

(row, column) to position in JavaScript:

```js
function posFromRowCol(r, c) {
  return (r - 1) * r / 2 + c;
}
```

Sanity check:

```js
for (let r = 1; r <= 5; ++r) {
  for (let c = 1; c <= r; ++c) {
    console.log(`r: ${r}, c: ${c}, position: ${posFromRowCol(r, c)}`);
  }
}
```

![row and column to position](img/math--rc2p.png)

## Precompute utility

A Python script that precomputes the game states and exports to JSON.

state2ascii:

![state2ascii](img/state2ascii.png)

Next possible future states of state 189:

![Next possible future states of state 189](img/future-states-189.png)


## Current components (tpg-spa/src)
- `Board` renders the triangular grid from a bitmask and forwards clicks with selection state.
- `Position` chooses Peg/Hole rendering per position and relays events.
- `Hole` renders an empty position and shows possible-move highlighting.
- `Peg` renders the peg visual and selection highlighting.
- `StatusBar` shows peg count, status text, and the end-of-game overlay.
- Logic helpers in `logic/` stay pure and do not touch React state directly (game math + lazy states loader).

## Prototype reference
- [UI prototype](prototype/triange_peg_game_ui_prototype.html) captures the sizing, spacing, and clip-path for the board/peg layout. The React components should mirror these proportions.
- Snapshot: [Prototype snapshot](img/tpg--ui-prototype.png).

## Implementation plan (current focus)
1) Bitmask utilities + static move list (JS module) to pair with the precomputed `states.json`.
2) Wire Board/Hole/Peg/Position to render from the bitmask and surface click events; keep the first-click “remove peg” flow. ✅
3) Move application, selection + legal target highlighting, peg count, and status messaging. ✅
4) Undo/reset controls, plus animation and responsive polish. ⏳
5) README/docs refresh, Vercel-ready build check, and tests beyond the states sanity check. ⏳

## Deployment target
- Vercel static hosting using `npm run build` output from Vite. Keep assets relative and avoid server-only APIs.
