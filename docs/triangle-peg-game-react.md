# triangle-peg-game-react – Coming Soon
Part of the [`cv-samples`](../README.md) portfolio. This is my React learning project, captured here as a coming-soon dossier for the portfolio gallery.

## Overview
I am building a single-page React implementation of the classic triangular peg solitaire puzzle (15-hole Cracker Barrel board: start with 14 pegs, one empty). Players tap/click pegs, see legal move highlights, and jump over adjacent pegs into empty holes until only one peg remains or no moves are left.

## Goals
- Demonstrate core React skills: functional components, hooks-based state, controlled inputs, derived state, undo/redo history.
- Keep game logic pure and isolated from the UI; components stay dumb while `logic/` handles moves and validation.
- Deliver a polished, responsive UI that fits both desktop and mobile, with simple animations for peg moves and highlights.
- Ship with a small README, build scripts, and Vercel-ready deploy instructions.

## Architecture
```
triangle-peg-game-react/
├─ src/
│  ├─ components/
│  │  ├─ Board.jsx        # renders triangular grid of holes
│  │  ├─ Hole.jsx         # individual hole with peg/empty state
│  │  ├─ Peg.jsx          # visual peg + selection/highlight
│  │  ├─ Controls.jsx     # reset, undo, show moves toggle
│  │  └─ StatusBar.jsx    # peg count, move availability, victory message
│  ├─ logic/
│  │  ├─ moves.js         # legal move list, generators, validators
│  │  └─ boardUtils.js    # apply/undo moves, board factories, helpers
│  ├─ App.jsx             # owns state, orchestrates events
│  └─ index.jsx           # entrypoint
└─ public/                # static assets, placeholder screenshot
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
- Tooling: Vite or Create React App (leaning Vite) with npm scripts for dev/build/test.
- Hosting target: Vercel static build; include `npm run build` guidance and `vercel.json` notes if needed.
- README will cover install, dev server, tests, and deployment steps.

## Backlog slices
1) Static board rendering with mock data and styling.
2) Hooked-up game state with selection + legal move highlighting.
3) Move application, history/undo, and status messaging.
4) Animations, accessibility polish, and responsive tuning.
5) README, screenshot placeholder, and Vercel deploy check.

## Related documents
- [Documentation hub](README.md)
- [Future work index](future_work.md)
