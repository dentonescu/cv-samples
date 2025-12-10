# Precompute utility
Python helper that emits the full 15-bit state space to `../tpg-spa/public/states.json` for the React app.

## Usage
```bash
python3 precompute.py --export-json ../tpg-spa/public/states.json
```

Optional flags:
- `--show-board <STATE_INTEGER>`: renders a state as ASCII.
- `--show-future-states <STATE_INTEGER>`: shows reachable next states for a given state.

`make`/`cmake` targets in the project root invoke this automatically before building the React app.
