# null-cipher

Single-page cipher demo that hides a message inside cover text by skipping through letters according to a numeric key. I built it to showcase how I explain classical crypto techniques with approachable UI and well-documented JavaScript.

[Live preview](https://cv-samples.vercel.app/null-cipher)

## Using the page
1. Open `index.html` directly in a browser or serve the folder locally (e.g., `python3 -m http.server`).
2. Enter the message in the “Input” box.
3. Supply the numeric cipher key. Each digit controls how many filler characters get inserted (encryption) or skipped (decryption). Example: a key of `3011` yields the skip pattern `+3, +0, +1, +1, +3, ...`.
4. Click **Encrypt** to generate the cover text. Random filler letters (A–Z) are inserted automatically.
5. Paste the cover text into the input box later, provide the same key, and click **Decrypt** to recover the hidden message.

The UI trims whitespace, uppercases the working text, and validates the key before running either mode. I surface friendly alerts any time validation fails so the interaction stays obvious for casual readers.

## Features
- Deterministic null-cipher algorithm with inline documentation in [`null-cipher.js`](null-cipher.js).
- Browser-only implementation—no build step or external dependency required.
- Random padding created per digit so every encryption run yields a different-looking cover text while remaining decipherable with the same key.
- Reset button that clears all fields to make back-to-back demos easy during interviews.

## Implementation notes
- All DOM glue and cipher logic live in [`null-cipher.js`](null-cipher.js); functions carry concise JSDoc blocks so the code doubles as documentation.
- The random padding source uses `Math.random()` intentionally for readability; the NOTES file tracks the idea of swapping in seeded randomness for reproducible explain-mode screenshots.
- UI markup stays in [`index.html`](index.html) with lightweight styling so the assets load instantly even from a `file://` URL.

## Development workflow
- Open `index.html` directly or run a throwaway static server when testing cross-origin behaviour.
- Update the cipher logic in `null-cipher.js`, keeping the doc comments accurate so the README, NOTES, and code never contradict each other.
- Reference [NOTES.md](NOTES.md) for the roadmap (explain mode, preset examples, accessibility polish) and the development timeline.

## Screenshot

![Null Cipher screenshot](null-cipher.png)

## Navigation
- Developer notes: [NOTES.md](NOTES.md)
- Portfolio overview: [../README.md](../README.md)
