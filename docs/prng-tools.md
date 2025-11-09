# prng-tools – Future Work & Research Notes
Part of the [`cv-samples`](../README.md) portfolio. The short-form summary lives in the [research incubator overview](../README.md#research-incubator-prng-tools).

## Overview
`prng-tools` is a long-range laboratory for studying pseudorandom number generators (PRNGs), entropy sources, and cryptographically secure randomness. The project focuses on comparable generators written in C, Python, Java, C#, Rust, plus an optional hardware RNG (HRNG) stream so experiments span both software and physical entropy.

## Motivation
We want empirical answers to three questions: how random the default generators across languages really are, how their “secure” APIs map onto operating-system CSPRNGs, and how hardware entropy compares when visualised and benchmarked alongside software streams. The lab emphasises reproducibility—automatable tests, shareable plots, and containerised runs.

## Goals
- Ship lightweight language services that expose random bytes via HTTP streaming or SSE so every generator produces uniform telemetry.
- Provide an analyzer-first workflow that buffers streams, runs statistical batteries (built-in plus Dieharder / NIST SP 800-22), and emits machine-readable reports and plots.
- Standardise the API contract (JSON metadata + binary chunks) so PRNGs and HRNGs plug in without special handling.
- Keep a deterministic control stream visible to validate dashboards and highlight bias.
- Reserve plumbing for a USB-CDC HRNG so hardware entropy can be compared live and optionally forwarded to `rngd`.

## Architecture
```mermaid
flowchart LR
    subgraph Software Generators
        C[C shim]\nlibsodium
        PY[Python FastAPI]
        JAVA[Java Spring SSE]
        CS[C# minimal API]
        RUST[Rust Axum]
    end
    HRNG[USB-CDC HRNG daemon]
    ANALYZER[Analyzer service\n(stat tests + reports)]
    WEB[Bootstrap/Plotly dashboard]

    C -->|HTTP/SSE bytes| ANALYZER
    PY --> ANALYZER
    JAVA --> ANALYZER
    CS --> ANALYZER
    RUST --> ANALYZER
    HRNG -->|Serial→SSE| ANALYZER
    ANALYZER -->|REST/SSE| WEB
```

### Repository layout
```
prng-tools/
├─ generators/          # Language-specific PRNG/CSRNG shims + HRNG collector
├─ analyzer/            # Statistical test harness, Dieharder/NIST adapters, report export
├─ web/                 # Bootstrap + Plotly UI (React upgrade later)
└─ docker-compose.yml   # Stack orchestration, analyzer workers, HRNG sidecar
```

## Statistical tests & math coverage
| Test | What it measures | Notes |
| --- | --- | --- |
| Monobit (frequency) | Balance of zeros vs ones | Drives quick histogram sanity checks |
| Runs test | Distribution of consecutive identical bits | Surfaces oscillation or bias |
| Chi-square | Deviation from expected symbol counts | Run at byte and nibble granularity |
| Serial correlation | Relationship between successive values | Highlights predictable generators |
| Shannon entropy | Information per bit/byte | Compared against the 1.0 bits/bit ceiling |
| External suites | Dieharder, NIST SP 800-22 | Analyzer shells out and ingests JSON/CSV logs |

## Entropy sources & OS integration
- **Linux:** prefer `getrandom()` with `/dev/urandom` fallback; document which libc/stdlib APIs (e.g., `std::random_device`) land on the kernel CSPRNG.
- **Windows:** use `BCryptGenRandom` (CNG). .NET and Win32 shims should log provider names for provenance.
- **macOS:** rely on `SecRandomCopyBytes` via `Security.framework` bindings for Swift, Objective-C, and Rust.
- **Language metadata:** Each generator advertises whether it wraps a deterministic PRNG (e.g., `xoshiro256**`) or delegates to the OS CSPRNG so the analyzer can annotate plots correctly.

## Hardware entropy milestone
- Microcontroller HRNG emits samples over USB-CDC serial; no kernel driver required across Linux/Windows/macOS.
- User-space collector (Rust or Python) reads `/dev/ttyACM*`/COM ports and republishes the same HTTP/SSE contract used by software generators.
- Optional modes feed the stream into `rngd` for host seeding or route it directly to the analyzer queue.

## Analyzer & frontend concept
- Analyzer batches incoming streams, schedules statistical jobs (synchronous for quick tests, queued workers for Dieharder/NIST runs), and persists summaries in SQLite/Parquet artifacts.
- Bootstrap + Plotly dashboard visualises histograms, entropy timelines, comparative scorecards, and stacked bar summaries per generator; React remains a future enhancement if interactions demand.
- Deterministic control streams stay visible beside live PRNG/HRNG sources to validate the rendering pipeline.

## Milestones / slices
| Slice | Focus |
| --- | --- |
| 1 | Python generator + analyzer skeleton + histogram smoke tests |
| 2 | Runs test and serial correlation added to analyzer reports |
| 3 | Second generator (Java or C) onboarded via the shared API contract |
| 4 | Deterministic control stream wired end-to-end |
| 5 | HRNG USB-CDC ingestion, comparison panes, and optional `rngd` hookup |
| 6 | Container images + published research notes for reproducible reruns |

## Academic relevance
`prng-tools` bridges systems engineering, applied cryptography, and statistics. Its containerised runs, cross-language experiments, and shareable datasets make it suitable for open research notes, student publications, or reproducible demonstrations.

## Future work
- Integrate broader PRNG batteries (PractRand, TestU01) alongside Dieharder/NIST.
- Benchmark throughput vs measured entropy per bit for each generator and hardware source.
- Compare HRNG streams and OS-backed CSPRNGs across Linux, Windows, and macOS hosts.
- Visualise spectral properties (FFT on bitstreams) to detect periodic components.
- Explore distributed agents that stream PRNG data from remote nodes for geographically diverse comparisons.

## Related documents
- [Future work index](future_work.md)
- [Top-level README overview](../README.md#research-incubator-prng-tools)

