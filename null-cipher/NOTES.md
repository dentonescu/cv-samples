# NOTES

Developer notes for the Null Cipher single-page app. Captures the roadmap, crypto references, and UI considerations used while iterating on the demo.

## Current File Layout
```
null-cipher/
├─ README.md                     # Project overview and usage notes
├─ NOTES.md                      # Developer plan and history
├─ index.html                    # Single-page UI shell
├─ null-cipher.js                # Cipher logic + DOM interactions
├─ null-cipher.png               # Branding/Screenshot asset
└─ .gitattributes
```

## Implementation Plan
1. **Core mechanics**  
   - Keep the cipher implementation (`null-cipher.js`) readable and well-commented; document how keyword removal and padding work.  
   - Add optional “explain” mode that highlights which letters are signal vs. noise.  
   - Provide preset examples (e.g., famous phrases) to showcase encoding/decoding quickly.
2. **UX improvements**  
   - Enhance mobile layout (responsive textarea sizing, bottom sheet for controls).  
   - Offer keyboard shortcuts for encode/decode/clear operations.  
   - Add theme toggle (light/dark) without extra dependencies.
3. **Validation & accessibility**  
   - Introduce unit tests (Vitest/Jest) for conversion routines.  
   - Add input sanitation for non-letter characters and surface helpful warnings.  
   - Ensure ARIA labels describe cipher steps; include copy-to-clipboard with success feedback.
4. **Documentation**  
   - Expand README with quick-start examples and references to classic null-cipher puzzles.  
   - Generate a small FAQ explaining the historical context and security caveats.  
   - Link to these notes from the README (see “Developer notes”).
5. **Deployment**  
   - Keep the build pipeline minimal (vanilla JS + HTML).  
   - Provide `npm` script (optional) for bundling/minification if the app grows.  
   - Document how the page is served in the portfolio (static hosting).

## Domain Notes
- Null (concealment) ciphers embed true messages inside cover text; this demo assumes uppercase letters mark the secret. Keep logic adaptable for other patterns (e.g., every nth character).  
- Ensure the algorithm remains deterministic; preserve character indices so decode operations can replay the same offsets.  
- When introducing randomness (for placeholder characters), seed the RNG for reproducible demonstrations.

## Implementation Timeline
- **2025-10-10 – 2025-10-11: Algorithm naming & framing**  
  Initial commit delivered the original “skip cipher” implementation, and the follow-up rename clarified the project as a null cipher demo with matching UI copy.
- **2025-10-27 – 2025-11-01: Documentation & repo hygiene**  
  Portfolio documentation contextualised the app, and `.gitattributes` alignment brought tooling in line with the rest of the samples. Functional code remains intentionally small and focused.

## Testing Checklist
- Add unit tests for encode/decode helpers with edge cases (empty input, punctuation, mixed case).  
- Run tests via `npm test` (if a toolchain is introduced) before publishing updates.  
- Manual smoke test in modern browsers (Chrome, Firefox, Safari).

## Related Documentation
- Project overview: [README.md](README.md)  
- Portfolio context: [../README.md](../README.md)
