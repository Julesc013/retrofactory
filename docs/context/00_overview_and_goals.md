# Retrofactory – Overview and Goals

## Project Summary

Retrofactory is a deterministic, Factorio-like factory and colony simulation game that runs on a wide spectrum of hardware and operating systems, from MS-DOS and early Windows to modern POSIX and Web.

A single core simulation engine is shared across all editions. Each edition is a different packaging of:

- Front-end platform code,
- Render backend choice(s),
- Performance profile(s),
- Content packs (base + planets + space).

## Primary Goals

- **Deterministic core**
  - All gameplay state updates are deterministic across platforms for a given input stream.
  - Fixed-point / integer numerics in simulation; no floats in state-changing code paths.
  - Centralised RNG with fully serialisable state.

- **Portability**
  - Core, world, schedule, save, runtime, and utility code must compile with old C++ compilers.
  - OS-, window-, and graphics-specific code is isolated under `source/platform` and `source/render`.

- **Extensibility**
  - Clear module boundaries: core, world, schedule, save, engine, mods, runtime, utility, render, interface, platform.
  - Content provided as data packs (base, planets, space) and later mods.
  - Room for script and native extensions without polluting core.

- **Replaceability**
  - Each major subsystem lives in its own directory.
  - It must be possible to swap:
    - Render backend,
    - Platform implementation,
    - Content pack,
    - Mod layer,
    without touching core simulation logic.

- **Retro friendliness**
  - Conservative CPU and memory usage.
  - Avoid heavy runtime features (exceptions, RTTI, heavy STL) in kernel modules.
  - Buildable with retro toolchains for MS-DOS and early Windows where possible.

## High-Level Architecture

- **Data**
  - `data/base` – core game content.
  - `data/{mercury,venus,mars,jupiter,saturn,space}` – planetary/space packs.
- **Code**
  - `source/system` – basic types and RNG.
  - `source/core` – core simulation systems (items, energy, logistics, networks, research).
  - `source/world` – world topology and grids.
  - `source/schedule` – tick phases and events.
  - `source/save` – save/load and deterministic state hashing.
  - `source/config` – profiles and versioning.
  - `source/engine` – host glue around the core.
  - `source/mods` – mod plumbing (data, script, binary).
  - `source/runtime` – config, logging, profiling, error, save manager.
  - `source/utility` – containers, strings, paths, hashing, fixed math, geometry.
  - `source/render` – software and GPU backends.
  - `source/interface` – UI, launcher, networking.
  - `source/platform` – OS-specific entrypoints and glue.

## Intended Use of These Context Files

- Loaded by code-generation tools (e.g. Codex) as always-on constraints.
- Define:
  - Which modules may depend on which.
  - Where new features should be implemented.
  - Language and determinism rules that cannot be violated.
