# Workflow, CI, Testing, and Roadmap

This document describes how to grow the codebase step by step and how CI/testing should be structured.

## Build Workflow

- Build system: CMake (or similar) with:
  - Root `CMakeLists.txt`.
  - Per-module CMake files under `source/`, `tools/`, and `tests/`.
- Primary dev targets:
  - POSIX desktop (Lx) using `source/platform/posix`.
  - Windows (9x/Nx) using `source/platform/win32`.
- Secondary targets:
  - MS-DOS (`source/platform/msdos`).
  - Win16 (`source/platform/win16`).
  - Web (`source/platform/web`).

`build/` is the canonical out-of-tree build directory.

## CI Expectations

- All tests under `tests/` must pass on primary dev targets.
- Determinism tests must run on:
  - At least one POSIX target.
  - At least one Windows target.
- Build must be warning-free (or minimal and tracked) in kernel modules.

## Test Layout (Conceptual)

`tests/` will eventually contain:

- `tests/core/` – tests for core and world:
  - World construction and mutation.
  - Item/energy/logistics/research behaviour.
- `tests/determinism/` – save/load, replay, and state-hash tests.
- `tests/perf/` – performance/UPS tests at different map scales.
- `tests/render/` – backend smoke tests (initialise, draw, teardown).
- `tests/macrosim/` – transit and macro AI tests.

Each test target is a separate executable linked against core libraries.

## Development Roadmap (Condensed)

Phase 0 – Skeleton & Build
- Create directory structure and stub CMake files.
- Implement minimal stubs for:
  - `system_base`, `rng`, `core_api`, `eng_app`, `plat_api`, `posix_main`.
- Get a no-op executable building and running (exit 0).

Phase 1 – Headless Core and Determinism
- Implement minimal `CoreState` in `core_api`.
- Integrate deterministic RNG.
- Implement `save` + `state_hash` for minimal state.
- Add determinism tests that:
  - Run N ticks.
  - Save/load.
  - Compare state and hash.

Phase 2 – World Grid and Simple Content
- Implement `world_space` and `world_grid` with a simple test world.
- Integrate world structures into `CoreState`.
- Extend save/hash to include world.
- Add tests under `tests/core/` for world construction and mutation.

Phase 3 – Items, Networks, Logistics, Research
- Implement:
  - `item`, `energy`, `logistic`, `networks`, `research`.
- Wire these into tick phases in `schedule`.
- Add tests for each subsystem and update determinism tests.

Phase 4 – Data Packs and Mod Loader (Tier 0)
- Implement:
  - `source/mods/data` loader, manifest, dependency graph, registry.
- Load `data/base` and one planet pack (e.g. `mars`) at startup.
- Add tests for:
  - Manifest parsing.
  - Mod ordering.
  - Prototype assembly.

Phase 5 – UI, Render, and Platform Integration
- Implement:
  - Basic `ui_system`, `ui_game`, `ui_debug`, `launch_main`.
  - `rnd_sw` renderer and selection via `rnd_pick`.
  - Platform-specific windowing/input on POSIX/Win32.
- Build a simple playable loop for Lx/Nx with:
  - Window.
  - Basic camera.
  - Simple HUD.

Phase 6 – Space Pack, Macro Layer, and Extended Platforms
- Integrate `data/space` content and transit/macro systems.
- Add more platforms (msdos, win16, web) as front-ends.
- Extend perf tests and determinism suites to cover macro cases.

## Guidance for Stepwise Development

When using code generation tools:

- Work phase-by-phase.
- For each phase:
  - Define a narrow goal (e.g. “headless core with deterministic tick”).
  - Limit changes to a small set of modules.
  - Require tests and build integration.
- After each phase:
  - Build.
  - Run tests.
  - Fix issues before expanding scope.

This controlled growth is mandatory to prevent architectural drift and keep the project maintainable and deterministic.
