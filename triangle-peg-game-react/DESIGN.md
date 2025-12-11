# triangle-peg-game-react – Design brief

Single-page React implementation of the classic triangular peg solitaire puzzle (15-hole Cracker Barrel board: start with 14 pegs, one empty). Players tap/click pegs, see legal move highlights, and jump over adjacent pegs into empty holes until only one peg remains or no moves are left.

## Goals
- Demonstrate core React skills: functional components, hooks-based state, derived state, undo/redo history.
- Keep game logic pure and isolated from the UI; components stay dumb while `logic/` handles moves and validation.
- Deliver a polished, responsive UI that fits both desktop and mobile, with simple animations for peg moves and highlights.
- Ship with a concise README, build scripts, and Vercel-ready deploy instructions.

## Architecture
```
tpg-spa/src/
├─ components/
│  ├─ Board.jsx        # renders triangular grid of holes + forwards clicks with selection state
│  ├─ Position.jsx     # selects Peg/Hole rendering per position
│  ├─ Hole.jsx         # individual hole with highlight for possible moves
│  ├─ Peg.jsx          # visual peg + selection highlight
│  ├─ Controls.jsx     # reset, undo, show moves toggle (planned)
│  └─ StatusBar.jsx    # peg count, move availability, victory message/overlay
├─ logic/
│  ├─ game.js          # bitmask helpers, board math, messages, move validation
│  └─ states.js        # lazy loader for precomputed states.json
├─ App.jsx             # owns state, orchestrates events
└─ index.jsx           # entrypoint
```

## Data model
- Board: `boolean[15]` (`true` = peg present) indexed 0–14 laid out as triangle rows (1/2/3/4/5).
- Move: `{ from: number, over: number, to: number }`.
- History: stack of prior board states for undo.
- Derived state: selected peg index, legal destinations, peg count, victory/no-moves messages.

## Gameplay rules
- A move is legal when `board[from] && board[over] && !board[to]` and the triple matches the triangular adjacency map.
- Applying a move: set `from=false`, `over=false`, `to=true`; push previous board to history.
- Undo pops history to restore board, selection, and derived state.
- Victory triggers when peg count is 1; “no moves left” when legal move set is empty.

## UI & UX
- Pegs are clickable/tappable circles; selection adds a glow; legal destinations glow blue.
- Board is centered with CSS grid/flex; scales for mobile widths without tiny tap targets.
- Simple CSS transform animation for jumps (e.g., translate along the triangle edge).
- Status bar surfaces peg count and guidance (“Move available”, “No moves left”, “You solved it!”).
- Controls: Reset, Undo, optional “Show legal moves” toggle for accessibility.

## Acceptance criteria
- All legal moves are enforced; illegal moves are ignored with clear visual cues.
- Board updates correctly after moves; jumped pegs disappear.
- Undo restores the previous state reliably.
- Reset returns to the canonical starting layout (1 empty hole).
- Peg count and status messages stay accurate.
- Responsive layout works on desktop and mobile.
- Code remains idiomatic React with clean separation between UI and pure logic.
- `npm test`/`npm run build` succeed; Vercel deploy instructions are documented.

## Deployment
- Tooling: Vite with npm scripts for dev/build/test.
- Hosting target: Vercel static build; include `npm run build` guidance and `vercel.json` notes if needed.
- README covers install, dev server, tests, and deployment steps.

## Backlog slices
1) Static board rendering with mock data and styling. ✅
2) Hooked-up game state with selection + legal move highlighting. ✅
3) Move application and status messaging; history/undo pending. ⏳
4) Animations, accessibility polish, and responsive tuning. ⏳
5) README, screenshot placeholder, and Vercel deploy check. ⏳
