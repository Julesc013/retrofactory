# Retrofactory File & Module Overview

This document lists every tracked source/documentation file in the repository (plus the generated build tree) and briefly explains its role. Entries are grouped by directory following the canonical layering rules captured in `docs/context/01_repo_layout_and_module_responsibilities.txt`.

---

## Repository Root

| File | Purpose |
|------|---------|
| `.gitignore` | Standard ignore list; keeps build artifacts and transient files out of version control. |
| `CMakeLists.txt` | Root CMake entry point; selects C++ standard and pulls in every module directory under `source/` and the global `tests/`. |
| `dir_structure.txt` | Human-readable dump of the original directory plan; useful for quick reference when creating modules. |
| `README.md` | Top-level project summary for developers and automation that land in the repo. |

### Generated Build Tree (`build_win/`)

The `build_win/` directory is produced by running `cmake -S . -B build_win` with the MSVC toolchain. It contains:

- Solution/project files (`retrofactory.sln`, `*.vcxproj`, `*.vcxproj.filters`) for every target.
- Intermediate CMake data (`CMakeCache.txt`, `cmake_install.cmake`, `CMakeFiles/**`).
- Object lists, incremental build stamps, and Visual Studio `.tlog` bookkeeping.
- Per-target build products under `build_win/source/**` and `build_win/tests/**` (Debug `.lib` archives and `.exe` binaries for the placeholder/determinism/core tests).

These files are regenerated on demand and intentionally excluded from source control.

---

## `docs/`

| File | Purpose |
|------|---------|
| `docs/context/00_overview_and_goals.txt` | High-level mission statement and constraints for Retrofactory. |
| `docs/context/00_project_vision_and_scope.txt` | Vision statement plus functional/non-functional scope. |
| `docs/context/01_directory_structure_and_responsibilities.txt` | Practical description of every top-level directory. |
| `docs/context/01_repo_layout_and_module_responsibilities.txt` | Detailed map of `source/**` modules and dependency ordering. |
| `docs/context/02_coding_constraints_and_style.txt` | Language level, coding style, naming, and determinism rules. |
| `docs/context/02_numerics_determinism_and_simulation_rules.txt` | Required numeric types, RNG discipline, scheduling, and hash rules. |
| `docs/context/03_editions_profiles_and_platforms.txt` | Edition/platform mapping and profile definitions. |
| `docs/context/04_mods_content_and_data_model.txt` | Content pack layout and mod loader expectations. |
| `docs/context/05_coding_rules_requirements_and_prohibitions.txt` | Hard requirements and forbidden behaviours for all modules. |
| `docs/context/06_workflow_ci_testing_and_roadmap.txt` | Build/CI workflow plus staged roadmap milestones. |
| `docs/file_overview.md` *(this file)* | Living inventory that explains every file in the repository. |

---

## `source/` (Code)

### `source/CMakeLists.txt`
Central build orchestrator for all code modules. Adds subdirectories in the mandated dependency order: `system → utility → world → runtime → core → render → interface → save → engine → platform`.

### `source/system/`

| File | Purpose |
|------|---------|
| `source/system/CMakeLists.txt` | Builds the `system` static library (currently only `rng.cpp`). |
| `source/system/system_base.h` | Canonical typedefs for fixed-width ints, tick types, and ID aliases for all kernel modules. |
| `source/system/rng.h` | Deterministic RNG interface (PCG-like state struct plus seed/next helpers). |
| `source/system/rng.cpp` | Implementation of the 64-bit LCG used everywhere in the core and world. |

### `source/utility/`

| File | Purpose |
|------|---------|
| `source/utility/CMakeLists.txt` | Builds the placeholder `utility` library; ready for future container/string helpers. |
| `source/utility/utility_stub.cpp` | Temporary stub to keep the target alive; returns a dummy value so linkers keep the lib. |

### `source/world/`

| File | Purpose |
|------|---------|
| `source/world/CMakeLists.txt` | Builds the `world` static library from spatial primitives. |
| `source/world/world_space.h` | Declares `World` and `WorldDimensions`, plus init/shutdown helpers. |
| `source/world/world_space.cpp` | Creates/destroys the default 8×8 chunks world and wires it to the grid allocator. |
| `source/world/world_grid.h` | Defines `Tile`, `Chunk`, fixed chunk constants, and tile accessor prototypes. |
| `source/world/world_grid.cpp` | Allocates and clears chunk arrays; implements `world_get_tile`. |

### `source/runtime/`

| File | Purpose |
|------|---------|
| `source/runtime/CMakeLists.txt` | Builds the `runtime` static library. |
| `source/runtime/rt_config_paths.h/.cpp` | Determines runtime directories (`./data`, `./config`, etc.). |
| `source/runtime/rt_log.h/.cpp` | Logging API; routes messages through the platform via `plat_log_message`. |
| `source/runtime/rt_error.h/.cpp` | Fatal error helper that logs then aborts (placeholder until structured handling arrives). |

### `source/core/`

| File | Purpose |
|------|---------|
| `source/core/CMakeLists.txt` | Builds the deterministic `core` library. |
| `source/core/core_api.h/.cpp` | `CoreConfig`/`CoreState` definitions (tick counter, RNG, embedded world) plus `core_init`, `core_tick`, `core_shutdown`. |

### `source/render/`

| File | Purpose |
|------|---------|
| `source/render/CMakeLists.txt` | Builds the `render` library composing API, picker, and software backend. |
| `source/render/rnd_api.h/.cpp` | Render front-end invoked by the interface; currently delegates every call to the software backend picked in `rnd_pick`. |
| `source/render/rnd_pick.h/.cpp` | Hard-codes the renderer selection to `RENDER_BACKEND_SW` (future home of capability checks). |
| `source/render/rnd_sw.h/.cpp` | Simple software renderer that fills an internal framebuffer with a color gradient and copies it into the platform surface. |

### `source/interface/`

| File | Purpose |
|------|---------|
| `source/interface/CMakeLists.txt` | Builds the `interface` library. |
| `source/interface/launch_main.h/.cpp` | Entry point-style loop: creates the platform window, seeds/ticks `CoreState` every frame, calls `render_frame`, and manages termination delays. |

### `source/save/`

| File | Purpose |
|------|---------|
| `source/save/CMakeLists.txt` | Builds the `save` library. |
| `source/save/save.h/.cpp` | Minimal `.rfs` serializer/deserializer supporting header, tick/RNG state, and full world tile payload (versioned). |
| `source/save/state_hash.h/.cpp` | Deterministic hash combining tick, RNG state, and world contents — used by determinism tests. |

### `source/engine/`

| File | Purpose |
|------|---------|
| `source/engine/CMakeLists.txt` | Builds the `engine` glue library. |
| `source/engine/eng_app.h/.cpp` | Very small façade that logs and defers control to `launch_run()`. |

### `source/platform/`

| File | Purpose |
|------|---------|
| `source/platform/CMakeLists.txt` | Adds per-platform subdirectories (currently only `posix`). |
| `source/platform/plat_api.h` | Platform abstraction: services struct, window/surface handles, event pumping, presentation, sleeping, and log forwarding. |
| `source/platform/plat_ids.h` | Enumerated platform IDs (currently POSIX). |

#### `source/platform/posix/`

| File | Purpose |
|------|---------|
| `source/platform/posix/CMakeLists.txt` | Builds the `platform_posix` library and the `retrofactory_posix_stub` executable. |
| `source/platform/posix/posix_platform.cpp` | Stub implementation of the platform API: allocates an in-memory pixel buffer, handles close requests, routes logs to stderr/`OutputDebugStringA`, and provides cross-platform sleeping. |
| `source/platform/posix/posix_main.cpp` | Program entry that initializes the platform, runs `engine_main_loop()`, and shuts down the platform. |

---

## `tests/`

| File | Purpose |
|------|---------|
| `tests/CMakeLists.txt` | Defines the placeholder executable plus determinism and world smoke tests; wires them into CTest. |
| `tests/placeholder_test.cpp` | Empty test harness used to keep CI wiring alive. |
| `tests/determinism/test_save_load.cpp` | Ensures save/load preserves tick, RNG, and hashes; exercises the `.rfs` format. |
| `tests/core/test_world.cpp` | Verifies world dimensions, tile writes, and that state hashes change/revert with tile edits. |

---

## README Reference (`docs/context` Mirror)

The open tabs mentioned in the IDE (`retrofactory_project_spec_master_v10.md`, `README.md`) mirror the specification content already embedded under `docs/context`. Keep those in sync if future edits happen outside the repo.

---

## Future Expansion Notes

- `source/config/`, `source/mods/`, `source/runtime/` extensions, and data pack directories (`data/base`, etc.) are currently placeholders. When files appear under those paths, extend this document with new entries.
- Additional platform front ends (Win32, web, DOS) should follow the same documentation pattern: add table entries under new subheadings within this file.
