# GitHub Automation

Continuous integration and repo-level automation live under this directory.  
Use it as the single reference when adjusting pipelines or adding new workflow files.

## Contents

| Path | Purpose |
| --- | --- |
| [`workflows/`](workflows/README.md) | GitHub Actions definitions, including the `cv-samples-ci` pipeline that mirrors `dev.sh`. |
| `.keep` / ancillary files | Reserved for future composite actions or shared workflow snippets. |

## Working with the workflows
- Prefer editing workflow files in a branch and running the equivalent `dev.sh` targets locally so the YAML stays in sync with the shell entrypoint.
- Re-run the `cv-samples-ci` workflow from the Actions tab before publishing major documentation or build changes; it exercises dependency install, builds, tests, and example runs for every project.
- Document any new workflow in [`workflows/README.md`](workflows/README.md) so navigation stays one click away.

## Navigation
- Portfolio overview: [../README.md](../README.md)
- Workflow details: [workflows/README.md](workflows/README.md)
