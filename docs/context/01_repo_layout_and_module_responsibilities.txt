

---

### `docs/context/01_repo_layout_and_module_responsibilities.md`

```markdown
# Repo Layout and Module Responsibilities

This document focuses on `source/` and how modules relate to each other. Use it to decide where to implement new functionality.

## `source/` Overview

```text
source
├── config
├── core
├── engine
├── interface
├── mods
│   ├── binary
│   ├── data
│   └── script
├── platform
│   ├── msdos
│   ├── posix
│   ├── web
│   ├── win16
│   └── win32
├── render
├── runtime
├── save
├── schedule
├── system
├── utility
└── world


source/system/

Lowest-level primitives shared across kernel modules.

Typical files:

system_base.h – fixed-width integer types, ID typedefs, unit constants.

rng.h / rng.cpp – deterministic RNG implementation and serialisation.

No dependencies on higher-level modules.

Must be buildable on all target compilers.

source/core/

Core simulation systems (the “kernel” gameplay logic).

Responsibilities:

Items and inventories.

Energy systems (power, fluids, heat).

Logistics (belts, inserters, basic logistics networks).

Generic network registry and dispatch.

Research and tech state.

Public core API (core_api.h/cpp) for engine access.

Depends on:

system/, utility/, and config/ (profiles/version).

Must not depend on:

render/, interface/, platform/, or OS headers.

source/world/

Spatial and macro topology:

Tiles, chunks, regions.

Colonies, surfaces, planets.

Transit layer abstraction:

Routes and manifests between colonies and planets.

Integrates with core systems only through well-defined interfaces.

source/schedule/

Tick and event management.

Defines:

Canonical tick phases and ordering.

Event queues and cross-system signalling.

Orchestrates calls into core/ and world/.

source/save/

Save/load and deterministic state hashing.

Responsibilities:

.RFS “Retrofactory Save” format implementation.

Versioned serialisation/deserialisation.

State hashing for replay and cross-platform determinism checks.

Must not perform IO directly tied to platform; uses runtime/platform abstractions.

source/config/

Core-side configuration structures.

Responsibilities:

Profiles (caps and algorithm tier selection).

Version constants for engine, save format, mod API.

Reads data from data/base/prototypes and config/ via runtime/engine.

source/engine/

Host layer around core:

Application lifecycle.

Configuration merge (config + data profiles).

Simulation loop.

Replay harness.

Core snapshot extraction for rendering.

Mod state tracking.

Depends on:

system/, core/, world/, schedule/, save/, config/, mods/, runtime/.

Exposes entrypoints used by interface and platform.

source/mods/

Subdirectories:

data/ – logic for Tier 0 data mod loading (manifests, dependency graph, registries).

script/ – placeholder for deterministic scripting VM and bindings.

binary/ – placeholder for native/binary mod ABI and loader.

Module responsibilities:

mod_api – C-level API surface exposed to mods.

mod_manifest – parse and validate mod metadata.

mod_loader – discover, load, and apply mods.

mod_depgraph – dependency resolution and conflict detection.

mod_registry – tracking loaded mods and capabilities.

source/runtime/

Platform-agnostic runtime helpers:

Config parsing and path resolution.

Logging.

Profiling.

Error reporting and crash messages.

Save slot management.

May call into platform abstractions via plat_* APIs but not directly into OS APIs.

source/utility/

Basic containers, string, path, hashing, fixed math, geometry.

Designed to be C++98-compatible and light-weight.

Used by core and other kernel modules to avoid heavy STL.

source/render/

Rendering backends and renderer selection:

rnd_api – engine-facing render entrypoints.

rnd_pick – backend selection based on config and capabilities.

rnd_sw – software renderer for retro and fallback.

rnd_gl1, rnd_dx5, rnd_gl2 – GPU backends for appropriate editions.

Consumes simulation snapshots provided by engine.

Must not modify core state.

source/interface/

UI toolkit, HUD, launcher, and network protocol.

Responsibilities:

Base UI system and skins.

In-game HUD and debug overlays.

Launcher flow plus mod/renderer selection.

Lockstep network protocol (client/server).

Uses:

Engine for simulation control.

Render module for drawing.

Platform abstraction for input/windowing.

source/platform/

Per-OS/platform implementation and entrypoints.

Subdirectories:

msdos – DOS 32-bit entrypoint and platform implementation.

win16 – Win16 entrypoint and platform implementation.

win32 – Win32 entrypoint and platform implementation, shared by 9x/Nx.

posix – POSIX entrypoint and platform implementation.

web – WebAssembly/Web platform implementation.

Shared files:

plat_api.h – abstract platform API surface (window, input, audio, FS, time).

plat_ids.h – platform and edition IDs.

tests/ and tools/

tests/ – test batteries for core, determinism, perf, render, macrosim.

tools/ – stand-alone tools that may link against core/libs but are not part of the runtime.