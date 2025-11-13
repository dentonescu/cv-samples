# Workflows

Automation scripts executed by GitHub Actions. Each workflow keeps parity with the local `dev.sh` entrypoint so CI and developer machines follow the same steps.

## `cv-samples-ci.yml`
- **Purpose:** End-to-end build, test, and example run for every portfolio project.
- **Phases:** installs multi-language dependencies (C toolchains, Python, Node.js), runs `./dev.sh --install-deps --build --run-tests`, and finishes with mock-mode demos so hardware requirements never stall CI.
- **Triggers:** pushes and pull requests targeting `main`; manual dispatch available for rehearsing releases.
- **Artifacts & checks:** surfaces compiler/test logs per project; mock Wi-Fi scans emit friendly logs when no interface is present.

## Adding new workflows
1. Consider whether the job can be expressed as a new subcommand in `dev.sh`; doing so keeps local/CI behaviour aligned.
2. Place the workflow YAML in this directory and document it here (purpose, triggers, dependencies).
3. Reference any new environment variables or secrets in the repository settings; never hard-code secrets in YAML.

## Navigation
- Back to [`.github`](../README.md)
- Repository overview: [../../README.md](../../README.md)
