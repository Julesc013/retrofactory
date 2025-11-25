# Retrofactory – Unified Project & File Specification
Version: context-spec v1  
Status: Design reference for Codex and all tooling

---

## 0. Global Project Specification

### 0.1 High-level goals

- Factorio-like, tile-based factory/colony game.
- Deterministic simulation (same inputs → same outputs across supported platforms).
- Single shared **simulation core** and data model, with:
  - multiple **editions** (1x, 3x, 9x, NX, LX, MX, CX, AX, WX),
  - multiple **render/present** backends,
  - multiple **platform layers** (MS-DOS, Win16, Win32, POSIX, WebAssembly).
- Heavy emphasis on **modding** and **content packs**:
  - All gameplay content shipped as data + Lua (no native binary mods).
  - Graphics/sound/music packs selectable and combinable like mods.
- Codebase must be:
  - portable, modular, and strictly layered,
  - easy to compile incrementally, test, and package for many targets.

### 0.2 Editions and platforms (short)

- **1x**: MS-DOS (16-bit/32-bit), text UI + CGA/EGA/VGA/VESA (software).
- **3x**: Windows 3.x, GDI + software/VESA.
- **9x**: Win95/98/ME, DirectX 5 + VESA.
- **Nx**: Win2000–10, DirectX 5 (where viable) and/or SDL2.
- **Lx**: Linux (32/64-bit), SDL2.
- **Mx**: macOS X (32/64-bit, PPC/Intel where feasible), SDL2.
- **Cx**: Classic Mac OS (7–9), QuickDraw.
- **Ax**: Android, platform TBD (later).
- **Wx**: WebAssembly, Web presenter.

### 0.3 Determinism and numerics

**Hard requirements for deterministic sim layers:**

- No `float`/`double` in:
  - `source/system`, `source/core`, `source/world`, `source/schedule`,
  - `source/saveload`, `source/mods`, `source/runtime` logic.
- Use only:
  - fixed-width integers (`int32_t`, `uint32_t`, `int64_t`…),
  - fixed-point helpers defined in `utility/fixed.h`,
  - deterministic RNG (PCG or similar) in `system/rng.[ch]pp`.
- No simulation logic may depend on:
  - wall clock time,
  - platform threads,
  - non-deterministic input sources (other than explicit command streams).

**Where floats are allowed:**

- Render layer (`source/render`).
- Presenters (`source/present`).
- Platform + UI for layout, coordinate transforms, interpolation.

### 0.4 Language and subset rules

**Sim and core layers:**

- Language: C++98 with a *very* C-like subset.
- Allowed:
  - Plain structs, simple classes as PODs,
  - Namespaces,
  - Inline functions,
  - Minimal templates only in `utility` (e.g., `Array<T>`).
- Forbidden in: `system`, `core`, `world`, `schedule`, `saveload`, `runtime`, `mods`:
  - Exceptions (`throw`, `try`/`catch`),
  - RTTI (`dynamic_cast`, `typeid`),
  - Heap allocation in hot loops without explicit strategy,
  - STL containers and algorithms (use `utility` instead).

**UI / render / platform / launcher:**

- Still C++98, but:
  - can use `new/delete` (controlled),
  - can use some STL in non-critical paths if really necessary (prefer not),
  - can use floats for render/UI math.

**Retro targets (1x/3x):**

- Any translation unit that ever links into DOS or Win3x binaries must be strict C89 (`.c`, no C++ features).
- This applies to `source/platform/dos16`, `source/platform/dos32`, `source/platform/win16`, presenters under `present/software`, `present/gdi`, `present/qdraw`, and launcher entry/UI used by 1x/3x (`launcher/entry/launcher_1x.c`, `launcher_3x.c`, `launcher/ui/text_ui.c`, `launcher/ui/gdi_ui.c`).
- Modern-only targets (9x+/NX/LX/MX/tools/tests) can keep the C++98 subset above.

**Lua:**

- Script mods are Lua only (no native code).
- VM binding is in `source/mods/script`.

### 0.5 Layering rules (strict)

From lowest to highest:

1. `system/` – types, ids, units, RNG.
2. `utility/` – arrays, bitsets, strings, paths, hashes, geometry, fixed math.
3. `core/`, `world/`, `schedule/`, `saveload/`, `mods/`, `runtime/` – sim & core runtime.
4. `engine/` – high-level glue, simulation driver, snapshot builder.
5. `render/` – sim-view rendering abstraction (no OS calls).
6. `present/` – backend-specific presentation (SDL2, DX5, GDI, QuickDraw, Web).
7. `platform/` – OS integration: window, input, audio, filesystem, net, clocks.
8. `interface/` – game UI, HUD, debug UI, network lockstep/client/server.
9. `launcher/` – launcher model, config, detection, per-edition entry points.
10. `tools/`, `tests/`, `config/`, `data/`, `external/` – around the core.

**Permitted dependency directions:**

- Each layer may depend on its own level and lower ones only.
- `core`/`world`/`schedule` may *not* depend on `render`, `present`, `platform`, `interface`, `launcher`.
- `render` depends on `engine` snapshots + `utility`/`system`/`data` access, not on `platform`.
- `present` depends on `render` (backbuffer/frames), `platform` (window, audio).
- `launcher` can depend on `platform`, `runtime`, `mods`, `config`, but **not** on sim internals.

### 0.6 Mod and content model

- All gameplay and packs are **data-only** + optional Lua scripting:
  - JSON/CFG for entities, recipes, research, worldgen, etc.
  - Graphics packs under `data/packs/gfx_*`.
  - Sound packs under `data/packs/sfx_*`.
  - Music packs under `data/packs/mus_*`.
- All official planet packs live under `data/{mercury,venus,earth,mars,jupiter,saturn,space}`.
- Mod loader:
  - scans `data/` and user `mods/`,
  - resolves dependencies,
  - builds a *canonical ordered list* of mods and packs,
  - merges prototypes according to priority (last wins for duplicates).

**Prohibitions:**

- No compiled/binary mod plugins (no `.dll`, `.so`, `.dylib`, etc.).
- No arbitrary `dlopen`/`LoadLibrary` from mods.
- No script access to OS, filesystem, or sockets outside approved API surface.

### 0.7 General prohibitions

- No platform-specific code in `core`, `world`, `schedule`, `saveload`, `mods`, `runtime`.
- No direct file I/O in sim – use `runtime`/`platform` for saves, configs, logs.
- No busy-waiting or sleeps in sim tick loop.
- No global singletons in sim state except clearly documented subsystems (RNG streams, id generators), and they must be save/load-consistent.
- No assumptions about pointer size (sim must work on 16/32/64-bit where feasible).

---

## 1. Directory & Module Overview (current tree)

Top-level (code-relevant):

- `config/` – cfg files per edition and defaults.
- `data/` – base game + planet packs + scripts.
- `external/` – third-party libs (SDL2, Lua, PCG, zlib).
- `source/` – all engine/game code (see below).
- `tests/` – unit tests, determinism tests, perf tests.
- `tools/` – offline utilities (asset packer, sprite builder, save viewer, etc.).

### 1.1 `source/` structure (current)

- `source/config` – game version and build-time config.
- `source/system` – types, ids, units, RNG.
- `source/utility` – array/vector/span/bitset/string/path/hash/fixed/geom.
- `source/core` – high-level sim context, entities, networks, recipes, research.
- `source/world` – world/planet/surface/colony/region/chunk/tile/transit/worldgen.
- `source/schedule` – events and tick scheduler.
- `source/saveload` – save/load, migration, sim state hashes.
- `source/runtime` – runtime config, logging, profiling, error handling, save manager, paths.
- `source/mods` – mod API, manifest, loader, depgraph, registry, script VM/bindings.
- `source/engine` – engine wrapper: config → core, tick, snapshot, replay, state.
- `source/render` – render API; software, DX5, GL stubs, backend selection.
- `source/present` – presenters: SDL2, DX5, software modes, GDI, QuickDraw, web.
- `source/platform` – per-OS glue: DOS16/DOS32/POSIX/Web/Win16/Win32.
- `source/interface` – UI core, skinning, HUD, debug, net lockstep/client/server.
- `source/launcher` – launcher model/config/detection + per-edition launcher entry.
- `source/tests` (under `tests/`) – see tests section.
- `tools/*` – offline tools.

---

## 2. File Specifications

Below: per directory, each file’s:

- Language + role.
- Declarations (main types, not every field).
- Functions (key API functions).
- Dependencies allowed.
- Typical dependants.

This is a **design brief**, not a final header; Codex should flesh out details within these constraints.

---

### 2.1 `source/config`

#### `version.h`

- **Language:** header-only, C/C++ compatible.
- **Purpose:** central version constants.

**Declarations**

- `#define RF_VERSION_MAJOR`
- `#define RF_VERSION_MINOR`
- `#define RF_VERSION_PATCH`
- `#define RF_VERSION_STRING "x.y.z"`
- `#define RF_SAVE_VERSION` – numeric save format version.

**Requirements**

- No includes beyond `<stdint.h>` / `<stddef.h>` if needed.
- No functions.

**Dependencies**

- None.

**Dependants**

- `saveload`, `runtime`, `engine`, tools, installer/build scripts (via CMake).

---

### 2.2 `source/system`

#### `types.h`

- **Purpose:** canonical basic types used across sim.

**Declarations**

- Basic aliases:
  - `typedef int8_t  i8;`
  - `typedef uint8_t u8;`
  - `typedef int16_t i16;`
  - `typedef uint16_t u16;`
  - `typedef int32_t i32;`
  - `typedef uint32_t u32;`
  - `typedef int64_t i64;`
  - `typedef uint64_t u64;`
- `typedef u32 Tick;` etc.
- Forward declarations of common structs (World, CoreContext, etc.) to break cycles.

**Functions:** none.

**Dependencies:** only `<stdint.h>`.

**Dependants:** literally everything else.

---

#### `ids.h`

- **Purpose:** typed IDs for all major entities.

**Declarations**

- `typedef u32 PlanetId;`
- `typedef u32 SurfaceId;`
- `typedef u32 ColonyId;`
- `typedef u32 RegionId;`
- `typedef u32 ChunkId;`
- `typedef u32 EntityId;`
- `typedef u32 TileId;`
- `typedef u32 ResearchId;`
- `typedef u32 RecipeId;`
- etc.

**Functions**

- Optional helpers:
  - `static inline bool entity_id_is_valid(EntityId id);`
  - etc.

**Dependencies:** `types.h`.

**Dependants:** `core`, `world`, `mods`, `saveload`, `runtime`, `render`, `interface`.

---

#### `units.h`

- **Purpose:** standard units and fixed-point scales.

**Declarations**

- Enums or constants for:
  - time tick (e.g. 1 tick = 1/60 s),
  - energy, power, temperature scales.
- Common conversion macros/inline functions:
  - `inline i32 joules_per_tick_from_kw(i32 kw);` etc.

**Functions:** a handful of small `inline` helpers.

**Dependencies:** `types.h`.

**Dependants:** `core`, `world`, `networks`, `research`, perf tests.

---

#### `rng.h` / `rng.cpp`

- **Purpose:** deterministic RNG streams (PCG or similar).

**Declarations**

- `struct RngStream { u64 state; u64 inc; };`
- Enum or constants for named streams (worldgen, events, etc.)

**Functions**

- `void rng_seed(RngStream* rng, u64 seed, u64 seq);`
- `u32 rng_next_u32(RngStream* rng);`
- `i32 rng_next_range(RngStream* rng, i32 min, i32 max);`
- `float rng_next_float01(RngStream* rng);` *(render/UI only; sim should use fixed)*

**Dependencies:** `types.h`.

**Dependants:** worldgen, events, research, tests, maybe UI randomness.

---

### 2.3 `source/utility`

This directory is reusable building blocks; all must be deterministic and platform-agnostic.

#### `array.h`

- Fixed or dynamically sized array wrapper.
- No exceptions, no RTTI, no STL.

**Declarations**

- Templates:
  - `template<typename T> struct Array { T* data; u32 size; u32 capacity; /*…*/ };`

**Functions**

- `void array_init(Array<T>*, Allocator*);`
- `bool array_reserve(Array<T>*, u32 new_capacity);`
- `bool array_push(Array<T>*, const T&);`
- `void array_clear(Array<T>*);`
- etc.

**Dependencies:** `types.h`.

**Dependants:** almost all modules (use with restraint in hot paths).

Similar pattern for:

- `vector.h` – SOA or 2D/3D small vector math (mostly for render/UI).
- `span.h` – non-owning slice wrapper.
- `bitset.h` – fixed/dynamic bitset.
- `string.h` / `string.cpp` – lightweight string buffer / string view.
- `path.h` / `path.cpp` – path join/normalise.
- `hash.h` / `hash.cpp` – hash functions for ids/elements.
- `fixed.h` – fixed-point arithmetic helpers (Qm.n).
- `geom.h` / `geom.cpp` – rectangles, points, view transforms for render/UI.

---

### 2.4 `source/core`

#### `core.h` / `core.cpp`

Already described in detail above.

**Language:** C++98, no exceptions/RTTI/STL.

**Dependencies**

- `system/types.h`, `system/ids.h`, `system/units.h`, `system/rng.h`
- `utility/*`
- `world/*`
- `schedule/*`
- `saveload/*`
- `mods/*`
- `runtime/*` (only via clean interfaces, e.g. save manager entry points as callbacks if needed)
- `config/version.h`

**Dependants**

- `engine/*`
- Some tests.

---

#### `entities.h` / `entities.cpp`

As specified above: prototypes + runtime state + per-chunk ticking.

**Dependencies**

- `system/*`, `utility/*`
- `world/chunk.h`, `world/tile.h`
- `mods/` for prototype data.

**Dependants**

- `core`, `world` (chunk tick), `render` (for prototype metadata), `saveload`.

---

#### `networks.h` / `networks.cpp`

As above: generic network system with per-type solvers.

**Dependencies**

- `system/*`, `utility/*`
- `world/*`, `entities.h`, `units.h`.

**Dependants**

- `core`, `world`, `tests`, `render` (for overlay data).

---

#### `recipies.h` / `recipies.cpp`

Recipe registry and helpers.

**Dependencies**

- `system/*`, `utility/*`
- `mods/*`, `research.h`.

**Dependants**

- `entities`, `UI`, tools, tests.

---

#### `research.h` / `research.cpp`

Research graph + state.

**Dependencies**

- `system/*`, `utility/*`
- `mods/*`, `saveload/*`.

**Dependants**

- `core`, `entities` (unlocks), `UI`, tests.

---

### 2.5 `source/world`

Files and functions were laid out above; summarising dependencies.

#### `world.h` / `world.cpp`

- Depends on: `system/*`, `utility/*`, `planet.h`, `surface.h`, `colony.h`, `region.h`, `chunk.h`, `tile.h`, `transit.h`, `worldgen.h`, `core` types.
- Called by: `core`, `engine`, `tests`, `tools`.

#### `planet.h` / `planet.cpp`

- Depends on: `system/*`, `world/surface`, `world/colony`, `world/transit`, `worldgen`.
- Called by: `world`, `tests`, `tools`.

#### `surface.h` / `surface.cpp`

- Depends on: `system/*`, `world/chunk`, `world/region`, `world/colony`.
- Called by: `planet`, `world`, `render` (for minimap), `tools`.

#### `colony.h` / `colony.cpp`

- Depends on: `system/*`, `world/surface`, `world/chunk`, `world/region`.
- Called by: `surface`, `world`, tests.

#### `region.h` / `region.cpp`

- Depends on: chunks, world stats.
- Called by: macrosim tests, tools, UI.

#### `chunk.h` / `chunk.cpp`

- Depends on: `tile`, `entities`, `networks`.
- Called by: `world`, tests, tools.

#### `tile.h` / `tile.cpp`

- Depends on: `system/*`.
- Called by: `chunk`, `worldgen`, render (for tile properties).

#### `transit.h` / `transit.cpp`

- Depends on: `world`, `entities`, `utility`.
- Called by: `planet`, macrosim tests, tools.

#### `worldgen.h` / `worldgen.cpp`

- Depends on: `world`, `tile`, `rng`, `mods` (worldgen prototypes).
- Called by: `world_init`, tests, tools.

---

### 2.6 `source/schedule`

#### `events.h` / `events.cpp`

As designed above.

**Dependencies**

- `system/*`, `utility/array.h` or `vector.h`.
- Event payload types must only include sim data (IDs, enums, small integers).

**Dependants**

- `core`, `world`, `research`, `engine` (for replays), tests.

#### `tick.h` / `tick.cpp`

As designed above.

**Dependencies**

- `system/*`
- `events`, `world`, `core`, `networks`, `research`.

**Dependants**

- `core`, tests (determinism).

---

### 2.7 `source/saveload`

#### `saveload.h` / `saveload.cpp`

- **Purpose:** Read/write full sim state to `.RFS` files.

**Declarations**

- `struct SaveDescriptor { /* path, slot id, etc. */ };`
- `bool saveload_read(const SaveDescriptor&, CoreContext* core, World* world, ResearchState* research, SaveMeta* meta_out);`
- `bool saveload_write(const SaveDescriptor&, const CoreContext* core, const World* world, const ResearchState* research, SaveMeta* meta_in);`

**Dependencies**

- `system/*`, `core`, `world`, `research`, `runtime/rt_paths`, `runtime/rt_log`, `config/version.h`, `hash.h`.

**Dependants**

- `core`, `runtime/rt_save_mgr`, tools (`save_viewer`), tests.

---

#### `migrate.h` / `migrate.cpp`

- **Purpose:** Upgrade older save versions to latest.

**Functions**

- `bool migrate_save(SaveFileRaw& inout, u32 from_version, u32 to_version);`

**Dependencies**

- `version.h`, `hash.h`, simple serialization logic.

**Dependants**

- `saveload`, `save_viewer`.

---

#### `hash.h` / `hash.cpp`

- **Purpose:** Deterministic sim state hashing (for replays & cross-platform tests).

**Functions**

- `u64 hash_world_state(const World&, const CoreContext&, const ResearchState&);`
- `u64 hash_rng_state(const RngStream*);`
- etc.

**Dependencies**

- `system/types`, `utility/hash`.

**Dependants**

- determinism tests, replay, save meta.

---

### 2.8 `source/runtime`

#### `rt_config.h` / `rt_config.cpp`

- **Purpose:** Parse `.cfg` files and expose runtime configuration.

**Declarations**

- `struct RuntimeConfig { /* video/audio/input paths */ };`
- `bool rt_config_load(RuntimeConfig*, const char* path);`
- `bool rt_config_merge(RuntimeConfig*, const RuntimeConfig& override);`

**Dependencies**

- `utility/string`, `utility/path`, `runtime/rt_log`.

**Dependants**

- `engine/config`, `launcher`, tools.

---

#### `rt_log.h` / `rt_log.cpp`

- Simple logging API.

**Functions**

- `void rt_log_init(LogSink* sink);`
- `void rt_log_info(const char* fmt, ...);`
- `void rt_log_warn(const char* fmt, ...);`
- `void rt_log_error(const char* fmt, ...);`

**Dependencies**

- platform logging callbacks indirectly (no direct OS calls here; platform provides sink).

**Dependants**

- almost all modules outside hot sim loops.

---

#### `rt_profile.h` / `rt_profile.cpp`

- Lightweight profiling: scoped timers, counters.

---

#### `rt_error.h` / `rt_error.cpp`

- Central error handling, error codes → user-facing messages.

---

#### `rt_save_mgr.h` / `rt_save_mgr.cpp`

- Save slot management, naming, path decisions.

---

#### `rt_paths.h` / `rt_paths.cpp`

- Resolves config/mod/save/pack directories.

All runtime files depend on `utility/*`, `system/*`, and talk to platform through pure C interfaces (no OS headers here).

---

### 2.9 `source/mods`

#### `mod_api.h`

- **Purpose:** C-level mod API surface; defines what Lua scripts can call.

**Declarations**

- `struct ModContext;`
- C API functions exposed to Lua, e.g.:
  - `int mod_api_spawn_entity(ModContext*, EntityProtoId, int x, int y);`
  - `int mod_api_set_recipe(...)`;
- No implementations here; just prototypes.

**Dependants**

- `script/scr_bind`, documentation, tests.

---

#### `manifest.h` / `manifest.cpp`

- Load and validate `manifest.json` for mods and packs.

**Functions**

- `bool mod_manifest_load(const char* path, ModManifest* out);`
- `bool mod_manifest_validate(const ModManifest*);`

**Dependencies**

- `utility/string`, `utility/hash`, external JSON parser.

**Dependants**

- `loader`, tools (`mod_sdk`), tests.

---

#### `loader.h` / `loader.cpp`

- Resolve mod list, load prototypes into `core`/`world`/`research`.

**Functions**

- `bool mod_loader_scan(ModRegistry*, const char* base_path, const char* user_path);`
- `bool mod_loader_resolve(ModRegistry*);`
- `bool mod_loader_apply(ModRegistry*, CoreContext*, World*, ResearchState*);`

**Dependencies**

- `manifest`, `depgraph`, `registry`, `saveload` (for mod settings), `runtime/rt_paths`.

**Dependants**

- `engine/config`, `launcher_model`.

---

#### `depgraph.h` / `depgraph.cpp`

- Dependency graph of mods; topological sort + conflict detection.

---

#### `registry.h` / `registry.cpp`

- Central list of active mods, their order, their provided content.

---

#### `script/scr_vm.h` / `scr_vm.cpp`

- Lua VM wrapper:
  - `bool scr_vm_init(ScriptVM*, const ScriptConfig&);`
  - `void scr_vm_shutdown(ScriptVM*);`
  - `bool scr_vm_run_mod(ScriptVM*, const ModScriptDesc&);`

#### `script/scr_bind.h` / `scr_bind.cpp`

- Bind `mod_api` functions into Lua state:
  - `bool scr_bind_core(ScriptVM*, ModContext*);`

---

### 2.10 `source/engine`

File names here are already “flattened” into:

- `engine.h` / `engine.cpp`
- `config.h` / `config.cpp`
- `replay.h` / `replay.cpp`
- `simulate.h` / `simulate.cpp`
- `snapshot.h` / `snapshot.cpp`
- `state.h` / `state.cpp`

Brief purposes:

#### `engine.h` / `engine.cpp`

- High-level entrypoint for running the game:
  - `bool engine_init(EngineContext*, const RuntimeConfig&);`
  - `void engine_shutdown(EngineContext*);`
  - `void engine_tick(EngineContext*, int frame_commands_count, const Command* cmds);`
  - `void engine_render(EngineContext*, RenderContext*);`

Depends on: `core`, `world`, `schedule`, `runtime`, `render`, `mods`, `saveload`.

#### `config.h` / `config.cpp`

- Bridge between `config/*.cfg` and `EngineConfig`.
- Loads edition config, render matrix, mod defaults.

#### `replay.h` / `replay.cpp`

- Record and replay of command streams + state hashes.

#### `simulate.h` / `simulate.cpp`

- Wraps sim tick loop; convenience for tests/CLI server.

#### `snapshot.h` / `snapshot.cpp`

- Builds `SnapshotWorld` structure consumed by `render`.

#### `state.h` / `state.cpp`

- Engine-level state machine:
  - titlescreen, in-game, loading, saving, etc.

---

### 2.11 `source/interface`

Files:

- `ui_core.[hc]pp` – widgets/layout primitives.
- `ui_skin.[hc]pp` – skin/theme config, fonts, colours.
- `ui_game.[hc]pp` – in-game HUD/build menus.
- `ui_debug.[hc]pp` – UPS, stats, debug overlays.
- `net_lockstep.[hc]pp` – lockstep MP protocol.
- `net_server.[hc]pp` – dedicated server network logic.
- `net_client.[hc]pp` – client network logic.

All of these:

- Depend on: `engine`, `render` (for metrics), `runtime`, `utility`, `mods` (for prototype display names), `system`.
- Must not depend directly on `platform` except via abstract input/audio APIs given by engine.

---

### 2.12 `source/launcher`

- `common/launcher_model.[hc]pp` – description of editions, packs, mods, renderers.
- `common/launcher_config.[hc]pp` – read/write launcher config.
- `common/launcher_detect.[hc]pp` – hardware/OS detection.

- `ui/text_ui.[ch]` – text launcher UI (DOS).
- `ui/sdl2_ui.[hc]pp` – SDL2 launcher UI (NX/LX/MX).
- `ui/gdi_ui.[ch]` – Win3x launcher UI.
- `ui/macosc_ui.[hc]pp` – classic Mac launcher UI.

- `entry/launcher_1x.c`, `launcher_3x.c`, `launcher_9x.cpp`, `launcher_cx.cpp`, `launcher_nx.cpp`, `launcher_lx.cpp`, `launcher_mx.cpp` – per-edition `main()` or entry stub.

Restrictions:

- Launcher must not embed sim code; it only:
  - detects capabilities,
  - chooses binary/exe, packs, mods,
  - writes config / command-line or calls into engine entry.

---

### 2.13 `source/render`

Already outlined above:

- `rend_api.[hc]pp` – public render API.
- `rend_pick.[hc]pp` – backend selection.
- `rend_sw.[hc]pp` – core software renderer.
- `rend_dx5.[hc]pp` – DX5 renderer.
- `rend_gl1.[hc]pp` / `rend_gl2.[hc]pp` – optional GL backends (can be stubbed for now).

Dependencies:

- `engine/snapshot`, `interface/ui_*`, `utility/geom`, `system/types`, packs metadata via `mods`/`data`.

---

### 2.14 `source/present`

Backend presenters:

- `sdl2/pres_sdl2.[hc]pp` – create SDL window, GL/renderer context; hand back buffer to `render`.
- `dx5/pres_dx5.[hc]pp` – create DX5 surfaces.
- `software/sw_core.[ch]`, `sw_cga.c`, `sw_ega.c`, `sw_vga.c`, `sw_vesa.c` – map software backbuffer to DOS/VGA hardware modes.
- `gdi/pres_gdi.[ch]` – Win16/Win3x GDI presenter.
- `qdraw/pres_qdraw.[ch]` – Mac classic QuickDraw presenter.
- `web/pres_web.[hc]pp` – WebAssembly/WebGL canvas presenter.

All depend on platform-specific headers/APIs; they must **not** call sim directly; only work with `RenderContext` and platform handles.

---

### 2.15 `source/platform`

Per-OS glue:

- `dos16/*` – 16-bit DOS entry and services.
- `dos32/*` – 32-bit DOS extender entry and services.
- `win16/*` – Win3.x entry, message loop, input/audio/fs/time.
- `win32/*` – Win95–10 entry, window, input/audio/fs/time/net.
- `posix/*` – Linux/macOS/BSD entry, POSIX I/O, SDL2 init, net.
- `web/*` – Emscripten entry and virtual FS.
- (Mac classic integration could appear as `macosc/*` if you split from posix.)

Each platform subdir provides:

- `*_main.cpp` – `main()` or `WinMain` or Emscripten entry; initialises platform, launcher/engine.
- `*_video.cpp`, `*_input.cpp`, `*_audio.cpp`, `*_fs.cpp`, `*_time.cpp`, `*_net.cpp` – implementations of a **platform API** declared in a shared `plat_api.h` (not yet in tree, but conceptually required).

---

### 2.16 `source/runtime`, `source/tests`, `tools/*`

Already covered conceptually; each tool has:

- `packer.[hc]pp`, `builder.[hc]pp`, `viewer.[hc]pp`, `inspect.[hc]pp`, `validate.[hc]pp`, `scaffold.[hc]pp`:
  - main entry + core logic;
  - depend on `core`, `world`, `saveload`, `mods`, `runtime`, `utility` as appropriate.
- Tests:
  - Should include the headers they exercise,
  - Use `engine/simulate` and `saveload/hash` for determinism.

---

## 3. Global Requirements & Prohibitions Summary

### 3.1 Requirements (global)

1. **Deterministic sim:** No floats, no time-dependent behaviour, fixed tick frequency, stable RNG.
2. **Strict layering:** Sim code has zero dependency on platform, render, present, or launcher.
3. **Portable build:** Same codebase builds for:
   - DOS16, DOS32, Win16, Win32, POSIX, WebAssembly.
4. **Mod/data-driven:** Base game and all planet content live in `data/`, loaded via `mods` subsystem.
5. **No native mods:** Only data+Lua. No external native code loaded at runtime.
6. **Multi-edition packaging:** Sim core identical across editions; only launchers, platform glue, and available backends differ.

### 3.2 Prohibitions (code)

- No OS headers or system calls in:
  - `system`, `utility`, `core`, `world`, `schedule`, `saveload`, `mods`, `runtime`.
- No exceptions or RTTI in sim code.
- No STL containers in sim code (`std::vector`, `std::map`, etc.).
- No dynamic allocation in inner sim loops without documented strategy.
- No direct render calls from sim (core/world/schedule cannot know about textures, fonts, etc.).
- No direct platform calls from UI; must go via engine/abstract input.

### 3.3 Prohibitions (content / packaging)

- No binary mod plugins.
- Packs and mods must be zip-based or plain directory trees with:
  - `manifest.json` + `prototypes/` + `graphics/` + `audio/` + `scripts/` + `locale/`.
- 1x/3x/9x/Cx edition directories and installer content must adhere to **8.3** filename constraints; repo names may be long, but build and packaging scripts must handle mapping.

---

This markdown document is the **single authoritative context** for:

- Project-wide constraints, goals, and layering.
- Responsibilities of each directory.
- Required declarations and functions in each file (at the conceptual level).
- Dependencies and dependants per module.

Codex and other tools should treat this as **hard constraints** when generating headers, implementations, and build files.
