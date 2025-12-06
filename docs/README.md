# Documentation Hub

Navigation index for the supporting documents that accompany the `cv-samples` portfolio. Use this page to jump between deeper design notes and future-work dossiers without manually scanning the folder.

| Document | Scope |
| --- | --- |
| [Future work index](future_work.md) | Master list of research tracks and incubating concepts; links to the longer design notes. |
| [prng-tools dossier](prng-tools.md) | Detailed plan for the cross-language PRNG/CSRNG lab, including architecture sketches and milestones. |
| [triangle-peg-game-react brief](triangle-peg-game-react.md) | Coming-soon React peg solitaire project plan with component structure, UX notes, and acceptance criteria. |
| [Terraglean notes](terraglean.md) | Concept brief and backlog for the planned mapping/analytics service. |

## How this folder is organised
- Each Markdown file in `docs/` focuses on a single research thread so it can evolve independently without bloating the project READMEs.
- Whenever a new incubator project appears, add it here and register it with the [future work index](future_work.md) so there are no isolated documents.
- Link back to the portfolio overview (`../README.md`) when summarising a project so casual visitors can contextualise the work quickly.

## Adding new documents
1. Create the Markdown file inside `docs/` (follow the existing naming pattern).
2. Update this README table and the [future work index](future_work.md) so navigation stays one click away.
3. Reference the new document from the relevant project README/NOTES so readers can discover it from multiple entry points.

_These documents are intentionally lightweight; keep them focused on design intent, future work, and context that would otherwise clutter the main READMEs._
