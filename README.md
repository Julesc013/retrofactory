# Retrofactory Project Specification Master

**Author:** Jules Carboni  
**Project:** Retrofactory 
**Version:** v10, 2025-11-17  
**Status:** Maybe out of date 

---

## 1. Vision and Scope

### 1.1 Vision

Retrofactory is a deterministic, Factorio-like factory and colony simulation game designed for **universal architectural compatibility**:

- From **8086/286/386** and **68k** systems through **Pentium-class** PCs and **PowerPC** Macs
- Up to modern **x86-64**, **ARM/ARM64**, **Apple Silicon**, and **selected consoles**
- With a **single canonical simulation engine (rf-core)** shared across all full editions

Key goals:

- **One game**, many front-ends:
  - Retro: MS-DOS, Windows 3.x, Windows 9x, classic Mac
  - Modern: Windows NT family, Linux, macOS, Android, Web (WASM)
- **Lockstep deterministic simulation**:
  - All full editions share the same rules and produce identical results under identical inputs (subject to caps).
- **Data-driven content**:
  - Base game (`rf-base`) + expansion(s) (`rf-space`) implemented as mods on top of rf-kernel.
- **Long-term survivability**:
  - Code and content designed to remain buildable and playable on decades of hardware/OS stacks.

### 1.2 Definitions (high-level)

- **rf-kernel**: the engine library; simulation, mod loader, runtime utilities, no platform APIs.
- **rf-core**: the official base game content (items, entities, recipes, tech, etc.), packaged as a **Tier-0** data mod.
- **rf-space**: official expansion mod that adds the space layer, orbital logistics, and planetary archetypes.
- **Planet packs**: mods that define specific planets/moons using the rf-space APIs.
- **Edition**: a packaged product line (1x, 3x, 9x, Nx, Lx, Mx, Cx, Ax, Wx) defined by platform front-end and profile caps.
- **Profile**: a data-driven configuration of caps and algorithm tiers for a given edition (e.g. Tier0_low, Tier1_mid, Tier2_high).
- **Colony**: a single local playable map (ground factory, orbital platform, space station, etc.).
- **Macro layer**: non-tile-level systems (planets, AI colonies, trade routes, etc.) driven by aggregate models.

---

## 2. High-Level Architecture

### 2.1 rf-kernel, rf-core, rf-space, and Mods

The system is structured as:

- **rf-kernel** (engine library)
  - Code in `src/core`, `src/engine`, `src/mods`, `src/runtime`, `src/util`.
  - Owns:
    - Core simulation loop and tick scheduler.
    - ECS/data models for entities/items/fluids/networks.
    - Save/replay (`.RFS`) format and migration.
    - Determinism rules, RNG streams, and invariants.
    - Mod loader and subsystem registration.
  - rf-kernel **knows nothing about specific game content** beyond generic systems.

- **rf-core** (base game mod)
  - Lives in `data/base`.
  - Defines:
    - Item and fluid prototypes, recipes, tech tree.
    - Machine classes, belts, inserters, power entities, etc.
    - Baseline planets and colony archetypes.
  - All “what is a furnace, what is an inserter” lives here.

- **rf-space** (space expansion mod)
  - Lives in `data/mods_official/rf_space`.
  - Adds:
    - Space layer and orbits.
    - Planetary archetypes and macro parameters.
    - Space-borne infrastructure and transit rules.

- **Planet packs & overhaul mods**
  - Planet packs: define concrete planets and scenario rules using rf-space APIs.
  - Overhauls: can replace rf-core content while still respecting rf-kernel interfaces.

Everything except rf-kernel is “just a mod”; rf-core + rf-space form the canonical game configuration.

### 2.2 Layered Code Architecture

Repository:

- `VERSION` – engine (rf-kernel) semantic version.
- `README.md` / `LICENSE`.
- `CMakeLists.txt` – entry point for the build.

Top-level directories:

- `docs/` – context notes, specs, and design history.
- `cfg/` – default configs and edition/profile presets.
- `data/` – base game content, official mods, user mods.
- `external/` – vendored third-party libraries (SDL, Lua, etc.) when needed.
- `build/` – out-of-tree build outputs.
- `dist/` – packaged SKUs (installers, ZIPs, floppies/CDs).
- `tools/` – asset packer, sprite builder, save inspector, mod SDK tools.
- `tests/` – deterministic, replay, perf, and render tests.
- `src/` – all engine code, split into layers:

`src/core/`  
: rf-core engine – simulation, world, networks, save, RNG, profiles  

`src/engine/`  
: host glue – config merge, logging integration, replay harness  

`src/mods/`  
: mod loader, manifest parsing, Tier-0/Tier-1/Tier-2 plumbing  

`src/runtime/`  
: shared runtime services – config parser, logging, profiling, save manager, error handling  

`src/util/`  
: low-level containers, string and path utilities, geometry helpers  

`src/render/`  
: rendering backends (software and GPU), consuming snapshots from rf-core  

`src/interface/`  
: UI toolkit, HUD, launcher, client/server control loops  

`src/plat/`  
: OS/platform abstraction (window, input, audio, files, time), per-edition front-ends  

Strict dependencies:

- `core` → `util` only.
- `engine` → `core`, `util`, `runtime`.
- `mods` → `core`, `engine`, `runtime`, `util`.
- `render` → `core` (via snapshot API), `util`.
- `interface` → `engine`, `render`, `runtime`, `util`.
- `plat` → `interface`, `render`, `runtime`, OS APIs.

rf-core never depends on `render`, `interface`, or `plat`.

---

## 3. Editions and CPU / OS Scope

### 3.1 Editions

Each **edition** is a packaged combination of:

- One or more **platform front-ends** from `src/plat`.
- A set of **edition profiles** (cap tiers).
- A bundled subset of **official mods**.

Canonical lines:

- **1x – MS-DOS**
  - MS-DOS 3–8 (8086→Pentium+).
  - CGA/EGA/VGA, optionally VESA.
  - 30–60 UPS, strict caps.

- **3x – Windows 3.0–3.11**
  - Win16, GDI graphics, MME audio.
  - Low-resolution software rendering.

- **9x – Windows 95/98/Me**
  - DirectDraw / DirectSound.
  - Optional GL 1.1 / DX5 class rendering.

- **Nx – Windows NT (2000/XP/7/10/12, 32/64-bit)**
  - Modern desktop edition.
  - Higher caps and full UI.

- **Lx – Linux**
  - SDL + OpenGL.
  - Similar to Nx in capabilities.

- **Mx – macOS**
  - macOS 10.0+ (GL or Metal wrapper).
  - Target parity with Nx/Lx.

- **Cx – Classic Mac**
  - System 7–9, 68k + PPC.
  - QuickDraw-based front-end.

- **Ax – Android**
  - GLES2+ mobile front-end.
  - Touch UI and trimmed UI complexity.

- **Wx – WebAssembly**
  - WASM build via Emscripten.
  - WebGL front-end.
  - Demo and (optionally) full edition with constrained profiles.

Experimental “mini” builds (8-bit, Z80, etc.) are treated as **separate projects** sharing design but not code.

### 3.2 Profiles and Algorithm Tiers

For each edition, data-driven **profiles** describe:

- Cap limits:
  - Number of colonies, entities, robots, network nodes, etc.
- Target UPS and recommended resolution ranges.
- Memory budgets (approximate).
- Algorithm tiers for subsystems:
  - `power_algo`: simple | advanced
  - `fluid_algo`
  - `heat_algo`
  - `transport_algo`
  - `ai_algo`
  - `worldgen_algo`

Examples:

- **Tier0_low** (1x/3x/9x baseline):
  - One colony, limited size.
  - Simple algorithms for all networks.
  - 30–60 UPS, 320×200 or 640×400 effective resolution.

- **Tier1_mid** (Nx/Lx low-end, Wx full):
  - Several colonies, medium caps.
  - Mixed algorithms (simple where safe, advanced where critical to feel).

- **Tier2_high** (Nx/Mx high-end):
  - Many colonies, large caps.
  - Advanced algorithms for key systems (e.g. power factor, richer fluid/heat, more AI).

Profiles are declared in:

- `data/base/prototypes/profiles.json` – canonical description.
- `cfg/profiles_*.cfg` – platform/edition-specific overrides (paths, UI defaults, etc.).

Profiles are immutable per save; loaders enforce profile compatibility.

---

## 4. Numeric Model and Determinism

### 4.1 Units and Types

rf-core uses integers and fixed-point representation only in simulation paths.

**Base units:**

- Time: **ticks** (hexiseconds; `1/60 s`)
- Position: `1/1024 m` sub-units
- Temperature: **milliKelvin**
- Mass: **grams**
- Volume: **millilitres**
- Energy: **Joules per tick**

**Canonical types:**

- Tile coordinates: `int32` (x, y) per colony.
- Sub-tile positions: fixed-point (e.g. Q22.10).
- Item counts: `int32`.
- Fluids: `int32` mL.
- Mass: `int32` g.
- Energy buffers/rates: `int32` (local, e.g. per entity), `int64` (global totals).
- Temperature: `int32` mK.
- Global tick: `uint64`.

Rules:

- No float/double in any code path that changes simulation state.
- No undefined behaviour:
  - No signed integer overflow on semantically bounded values.
  - No use of uninitialised memory, strict aliasing respected.
- Overflow decisions are explicit: clamp, wrap, or fail deterministically.

### 4.2 Determinism Rules

Determinism is non-negotiable for rf-core and any Tier-0/Tier-1 mods:

- Stable iteration order:
  - Arrays or index-sorted containers only in hot sim loops.
  - No reliance on unspecified iteration over hash maps.
- Single canonical tick order:
  - Well-defined sequence of phases (input, logistics, machines, networks, AI, cleanup).
  - Parallel execution permitted only if it is provably observationally equivalent.
- Platform and compiler constraints:
  - No platform-dependent `sizeof` layout for on-disk or network formats; all serialized structs are **packed, versioned, and endian-normalized**.
- RNG:
  - All randomness uses explicit RNG streams (e.g. PCG32) seeded from save.
  - RNG streams are part of serialized state and are reloaded on load/replay.
- CI:
  - Reference replays run across compilers/platforms must match per-tick hash or periodic hash checkpoints.
  - Any change that breaks deterministic test suites is a regression.

---

## 5. Language, Toolchains, and Coding Rules

### 5.1 Language Rules

rf-core and shared engine code:

- Use a restricted **C++98 subset** (“C with classes”).
- Allowed:
  - POD structs and simple classes.
  - Namespaces.
  - Function overloading.
  - RAII for lifetime management in non-hot paths.
  - Small, simple templates (e.g. `UfArray<T>`).
- Forbidden in rf-core/engine:
  - Exceptions (`throw`, `try`/`catch`).
  - RTTI (`dynamic_cast`, `typeid`).
  - Heavy STL use in hot paths.
  - Non-deterministic language features and UB.

Platform and tooling code (`src/plat`, `tools`) may use more modern C++ and STL as needed, with a **hard boundary**: no platform headers or STL types are allowed to leak into rf-core public headers.

### 5.2 Toolchains

rf-kernel must build with:

- **Linux dev/CI**:
  - GCC and Clang (modern versions).
- **Windows NT (Nx)**:
  - MSVC (supported recent version).
  - MinGW-w64 variant as secondary.
- **Retro targets** (later roadmap phase):
  - OpenWatcom for DOS/Win3.x.
  - Emscripten (WASM) for Web.
  - 68k/PPC toolchains for Classic Mac.

Compiler versions and flags are pinned in a build document. Any change to supported toolchains requires:

- CI green across all targets.
- Determinism regression tests passing.

---

## 6. World and Spatial Model

rf-core owns the entire world representation:

- Planets and surfaces.
- Colonies and local maps (tile/chunk/region).
- Macro space/planet relations.

### 6.1 World Hierarchy

Hierarchical structure:

- **Tile** – smallest addressable grid cell.
- **Chunk** – fixed block of tiles (e.g. 32×32).
- **Region** – fixed grid of chunks (e.g. 8×8 chunks).
- **Colony** – local playable map; comprised of regions/chunks/tiles.
- **Surface** – logical group of colonies (e.g. planet ground, orbital belt).
- **Planet** – owns one or more surfaces.
- **Space layer** – above planets, enabling interplanetary logistics and macro simulation.

Colony contexts:

- Ground factory.
- Orbital platform (around a given planet).
- Space station.
- Other special scenarios (e.g. asteroid base).

Each colony is a self-contained context; no direct pointers to other colonies. Cross-colony interactions happen through **transit manifests** and macro systems.

### 6.2 Planetary Surfaces and Colony Placement (Merged 5.4)

At the highest level, each **planet** owns a logical surface description independent of any single colony map.

#### 6.2.1 Surface Heatmaps

For each planet, rf-core generates several low-resolution, long-wavelength scalar fields over the planetary surface, e.g.:

- `H_elev` — elevation
- `H_temp` — temperature
- `H_moist` — moisture/wetness
- `H_rough` — terrain roughness/slope
- `H_pollution_base` — baseline environmental hostility
- `H_ore_i` — coarse richness fields per resource family

These fields are:

- Continuous over planet-normalized coordinates.
- Stored at a much lower resolution than colony tile maps.
- Used as inputs to colony worldgen, not simulated per-tick.

#### 6.2.2 Biomes and Surface Types

Biomes are defined as predicates over overlapping heatmaps:

- Examples: desert, savanna, grassland, forest, swamp, tundra, ocean, island chains.
- Each biome template maps `(H_elev, H_temp, H_moist, H_rough)` to:
  - Tile palette probabilities.
  - Entity (tree, rock, hazard) spawn weights.
  - Resource weight multipliers for each `H_ore_i`.
  - Colony-level modifiers (bonuses/penalties) used by macro systems.

Biomes are evaluated when:

- Previewing potential colony sites.
- Instantiating a new colony map.

#### 6.2.3 Colony Placement Rules

- Each colony is anchored to a **planet-space coordinate**.
- Hard separation constraint:
  - No two colonies may be placed within a configurable minimum distance (e.g. 128 km) on the planet.
- Colony types:
  - **Inland**: no direct sea access; may have lakes/rivers; no ocean shipping.
  - **Coastal**: has shoreline; can host seaports and ocean shipping routes.
  - **Oceanic/Island**: primarily islands/ocean; ground transport constrained.

Colony worldgen samples:

- Local values of all heatmaps at the chosen coordinate.
- Biome templates.

These drive tile/chunk generation to approximate the large-scale planetary context (continents, climate zones, etc.) without storing an entire planet’s worth of tile data.

#### 6.2.4 Single-Colony Editions

For editions limited to a single colony (e.g. 1x/3x, Wx demo):

- The planet still exists conceptually, but only **one colony** is allowed.
- Worldgen is biased so the single colony:
  - Includes compressed biome diversity, and/or
  - Features stronger gradients in heatmaps,
so a single map offers varied gameplay that stands in for the whole planet.

### 6.3 Transit and Macro Geography

rf-core defines **transit graphs**:

- Nodes:
  - Colonies, ports, orbital stations.
- Edges:
  - Shipping lanes, rail corridors, air routes, space corridors.

Transit manifests:

- Aggregate packets representing cargo, passengers, or other flows.
- Operate on macro time steps (multiples of ticks) but snap to the global tick grid.
- Are **not** individual items; they are aggregated, deterministic flows.

Macro geography (inter-colony, inter-planet trade) uses these transit graphs and persists them in save state.

---

## 7. Simulation Systems (rf-core)

rf-core contains the generic systems required for any Factorio-like factory game, abstracted away from specific content.

### 7.1 Items, Fluids, and Inventories

**Item prototypes**:

- ID, mass, volume.
- Stack size and density.
- Flags (flammable, radioactive, fluid-compatible, hazardous, etc.).

**Item stacks**:

- `(item_id, count)` with integer counts.
- No fractional items; probabilistic behaviour uses RNG on integer counts over time.

**Fluids**:

- Defined similar to items, but with volume, temperature, and phase properties.
- Stored in tanks/pipes as discrete integer mL amounts.

**Inventories**:

- Typed inventories (e.g. machine input, output, storage).
- Fixed slot arrays; per-slot or aggregated constraints.
- All “on ground” items are represented as entries in some inventory or transport structure (no free-floating items).

### 7.2 Entities and Machines

rf-core models:

- Static entities: furnaces, assemblers, labs, boilers, miners, etc.
- Dynamic entities: inserters, belts, robots, vehicles.

Entities track:

- Position, orientation, tile occupancy.
- Network membership (power, fluid, heat, logistics).
- Operating state (running, idle, broken, sleeping).

Machine logic:

- Recipes or behaviours defined by rf-core data + mod prototypes.
- Deterministic state transitions.
- Sleep/wake triggers to avoid unnecessary per-tick execution.

### 7.3 Logistics: Belts, Inserters, and Bots

**Belts:**

- Represented as discrete lanes or abstracted segments depending on profile.
- Each segment holds a queue of item descriptors or aggregated counts.
- Tick updates can be per-segment, per-lane, or bucketed using event lists.

**Inserters:**

- Deterministic movement; can be:
  - Simple: single-stage transfer per tick.
  - Advanced: multi-stage, with acceleration and kinetic constraints for high profiles.
- Sleep when idle: only wake on relevant changes (inventory updates, signals).

**Bots (where enabled):**

- Logistic bots are aggregated where possible:
  - Shared path caches.
  - Pooled states.
  - Aggregated delivery tasks.
- Pathfinding uses discrete nav graphs, with tiered complexity by profile.

### 7.4 Power Systems

Power networks are:

- Graphs of generators, consumers, and storage elements.
- With per-tick evaluation of:
  - Supply and demand.
  - Losses and storage behaviour.

Algorithm tiers:

- **Simple**:
  - Scalar power flow.
  - One voltage, no power factor.
- **Advanced**:
  - Separate LV/HV; optional AC/DC distinctions.
  - Power factor introduced as an efficiency term (not full electrical simulation).

Power networks are aggregated into **networks** that can sleep when static.

### 7.5 Fluid Systems

Fluid networks:

- Node/edge graphs (tanks, pipes, junctions).
- Use discrete steps of volume and pressure.
- Flow is computed deterministically with bounded complexity:
  - Simple edition: basic equalization algorithm.
  - Advanced: improved distribution with per-segment constraints.

No full Navier–Stokes; this is a design, not a CFD solver.

### 7.6 Heat Systems

Heat networks:

- Nodes with heat capacity and temperature, edges with conduction coefficients.
- Simple discrete conduction function per tick.
- Optional advanced behaviours (e.g. phase change) in higher profiles.

### 7.7 Research, Labs, and Science

Science and tech:

- Research items as discrete “packets” or bytes.
- Global tech tree; per-colony labs and local modifiers.
- Labs accumulate science and feed global tech.

Research progression:

- Deterministic; no random unlocks in core.
- Allows for later expansion with macro policies and AI by mods.

---

## 8. Networks and Algorithms (Requirements)

rf-core defines **network types**:

- **Power networks**
- **Fluid networks**
- **Heat networks**
- **Transport networks** (belts, rails, roads)
- **Data/logic networks** (where enabled)

Requirements:

- Each network type must:
  - Scale to expected caps for Tier2_high profiles.
  - Degrade gracefully (simpler algorithms) for lower tiers.
  - Maintain determinism and stable iteration order.

Algorithms must:

- Use bounded per-tick work relative to network size.
- Prefer incremental updates and event-driven mechanisms over full recomputation.
- Allow sleeping of stable networks.

Network SRS (condensed):

- Well-defined creation, merging, splitting, and destruction semantics.
- Clear ordering vs other systems (e.g. power before machines, fluids after machines).

---

## 9. Macro Layer: AI, Economy, and Abstraction

The macro layer is **minimal but extensible** in rf-core:

- **AI colonies**:
  - Modeled as abstract producers/consumers.
  - No full micro simulation; just demand/supply nodes.
- **Routes & trade**:
  - Defined as edges in the transit graph.
  - Move aggregate manifests between nodes.
- **Policies**:
  - Gated by tech and mod APIs.
  - Affect macro parameters (trade tariffs, environmental rules).

Abandoned or “background” colonies:

- May be simulated at reduced resolution:
  - Coarse production/consumption approximations.
  - State compressed into a small set of summary variables.

The macro layer is deliberately kept simple in v1, with hooks for expansions in rf-space and mod APIs.

---

## 10. Mods and Tiers

### 10.1 Tier Definition

- **Tier 0 – Data mods (v1)**
  - Prototypes and data only.
  - No code execution.
  - Deterministic by construction.
  - Supported on all full editions.

- **Tier 1 – Script mods (future)**
  - Deterministic VM (Lua-like) under `engine/script`.
  - Strict sandbox:
    - No IO, no wall-clock, no threads.
    - Bounded instructions per tick.
  - Allowed on Nx/Lx/Mx/Ax/Wx-full when explicitly enabled.
  - Forbidden in deterministic multiplayer and retro editions.

- **Tier 2 – Native mods (future)**
  - C ABI plugins.
  - Potentially non-deterministic and platform-specific.
  - Disabled by default, never allowed on:
    - Retro editions.
    - Wx (Web).
    - Lockstep multiplayer sessions.

### 10.2 Mod Manifests and Loader

Mod manifests include:

- Identity (name, version, author).
- Dependencies and conflicts.
- Required rf-kernel version range.
- Required capabilities (space, macro, PF, etc.).
- Supported editions and profiles.

Loader responsibilities:

- Scan mod directories:
  - `data/mods_official/`
  - `data/mods_user/`
- Parse manifests.
- Resolve dependency graph and load order deterministically.
- Load Tier-0 data into rf-core prototypes.
- Enforce edition/profile restrictions.

rf-core does not know where prototypes come from; it only sees the resolved, loaded prototype tables.

---

## 11. Front-Ends, Platforms, and Rendering

### 11.1 Platform Layer (src/plat)

Platform subdirectories implement `plat_api`:

- `dos1x/` – DOS 3–8 (CGA/EGA/VGA, optional VESA, PC speaker/SB).
- `win3x/` – Windows 3.x (GDI, MME audio).
- `win9x/` – Windows 95/98/Me (DirectDraw/DirectSound, GL 1.1 / DX5).
- `winnt/` – Windows 2000/XP/7/10/12 (GL1/2/4, DX5/9/11, SDL).
- `linux/` – Linux/BSD (SDL + OpenGL).
- `macx/` – Mac OS X (GL or Metal wrapper, CoreAudio).
- `classicmac/` – System 7–9 (QuickDraw).
- `web/` – WebAssembly + WebGL.
- `android/` – Android + GLES2+.

Each backend provides:

- Window/surface creation and resize.
- Input polling (keyboard, mouse, controllers, touch).
- Audio output.
- Filesystem mapping (config/saves/mod dirs).
- Time (ticks, monotonic clock).
- Error and log sinks.

They do **not**:

- Modify game rules.
- Implement game logic.
- Bypass rf-kernel for simulation changes.

### 11.2 Rendering (src/render)

Render layer backends:

- `sw/` – software renderer (CGA/EGA/VGA; chunk-based sprites, low-res UI).
- `gl1/` – OpenGL 1.x fixed-function pipeline renderer.
- `dx5/` – DirectDraw/Direct3D 5 renderer for Win9x.
- `gl2/` – OpenGL 2.x programmable pipeline for modern desktops.

Later potential backends:

- `vk/` – Vulkan.
- `dx11/` – Direct3D 11.

rf-core exposes **render snapshots**:

- Lists of visible entities, tiles, overlays.
- Camera and zoom state.
- UI directives (HUDs, panels, etc.).

Render backends read snapshots and draw frames. Platforms are responsible only for present/blit.

---

## 12. Interface Layer and Networking

### 12.1 UI Toolkit and Game UI

`src/interface` owns:

- UI primitives:
  - Panels, lists, buttons, labels, checkboxes, etc.
- Theming:
  - Multiple skins, colour schemes.
- Game UIs:
  - Main HUD, quickbars, entity windows.
  - Overlays for power, fluids, logistics, etc.
- Debug UI:
  - UPS counters, profiler overlays, network inspectors.

### 12.2 Launcher

Launcher responsibilities:

- Edition and profile selection.
- Mod selection and load order:
  - List installed mods.
  - Enable/disable.
  - Show dependency/conflict auras.
- Video options:
  - Resolution, fullscreen/windowed, renderer choice.
- Audio options:
  - Volumes, output device selection (where possible).
- Input options:
  - Keybindings, controller configurations.

### 12.3 Networking

Networking is **lockstep deterministic** for v1:

- Server stores authoritative state.
- Clients send input streams only.
- Server replays inputs and sends periodic hashes for verification.

Requirements:

- Same rf-core rules everywhere; no “simplified” client simulation.
- Tick alignment across peers.
- Configurable rollback window for latency tolerance (future).

Dedicated servers:

- Headless binaries using rf-kernel and minimal platform front-end.
- No rendering; logging and networking only.

---

## 13. Tooling, Packaging, and CI

### 13.1 Tools

Tooling targets under `tools/`:

- `asset_packer`:
  - Packs raw art into engine-friendly sprite sheets and metadata.
- `sprite_build`:
  - Builds animation atlases and masks.
- `save_view`:
  - Introspects `.RFS` saves for debugging and mod development.
- `world_inspect`:
  - Queries world state for metrics (e.g. number of belts, UPS cost).
- `mod_sdk`:
  - Validates mod manifests and prototypes.
  - Scaffolds new mods.

Tools may use higher-level C++ and STL; determinism is not required for tools.

### 13.2 Packaging & SKUs

Packaging rules:

- All builds reproducible via CI scripts.
- No manual ad-hoc packaging.

Representative SKUs:

- Retro:
  - 1x/3x floppy images or ISOs.
  - 9x single CD ISO.
- Classic Mac:
  - Cx disk images (System 7–9).
- Modern:
  - Nx, Lx, Mx installers and ZIPs.
- Gold:
  - Multi-edition DVD or installer bundle.
- Digital:
  - Standalone per-edition ZIPs/installers.

Directory layout inside installs:

- `RETROFACTORY/bin/` – executables.
- `RETROFACTORY/data/` – base game content.
- `RETROFACTORY/mods_official/` – official expansions.
- `RETROFACTORY/mods/` – user mods.
- `RETROFACTORY/saves/`
- `RETROFACTORY/config/`
- `RETROFACTORY/docs/`

### 13.3 CI & Testing

Mandatory CI suites:

- **Determinism tests**:
  - Known seeds and input streams.
  - Cross-compiler/platform replays must match hashes.

- **Worldgen stability**:
  - Regenerate worlds with same seeds; chunk data must match.

- **Transit and macro stability**:
  - In-flight manifests must survive save/load identically.

- **Performance budgets**:
  - Standardized test maps per tier.
  - UPS threshold checks.

- **Profile enforcement**:
  - No profile may exceed its declared caps.

---

## 14. Implementation Roadmap (Condensed)

From `plan.md` and later roadmap notes:

**Phase 0 – Skeleton & Build**

- Create repository structure.
- Implement bare-bones rf-core API stubs.
- Implement simple logging and config.
- Build one desktop target (e.g. Nx on Linux/Windows).
- Introduce basic headless loop + smoke test.

**Phase 1 – Minimal World and Save**

- Implement minimal world model (one colony, tiles, chunks).
- Implement items/inventories and simple machines.
- Implement `.RFS` v1 with hashing.
- Implement determinism tests and replay harness.

**Phase 2 – Logistics and Power**

- Belts + inserters with simple algorithms.
- Basic power network, simple scalar power.
- Stress/perf tests and caps for Tier0_low.

**Phase 3 – Fluids, Heat, and Science**

- Implement fluid networks, simple heat graphs.
- Implement labs and tech progression.
- Add more robust profiles and edition mapping.

**Phase 4 – Mods and UI**

- Finish Tier-0 mod loader.
- Implement full launcher UI.
- Implement core HUD and debug overlays.

**Phase 5 – Multi-Edition Front-Ends**

- Add Win9x, macOS, Linux, Web, and Android front-ends.
- Port UI and renderer to required platforms.
- Lock down profile configs per edition.

**Phase 6 – rf-space and Macro Layer**

- Implement space layer and transit graphs.
- Integrate planetary surfaces and macro features.
- Introduce planet packs.

Future phases (beyond v1):

- Tier-1 scripting, Tier-2 native mods.
- Enhanced thermodynamics and macro economy.
- Deeper AI, policies, and scenario systems.

---

## 15. Governance and Change Control

- This document (`retrofactory_project_spec_master_v10.md`) is the canonical spec.
- Any change to mechanics, numerics, or architecture:
  - Must be described in a spec delta or RFC.
  - Must be merged with an updated vX document.
- Semantic versioning:
  - rf-kernel, data schema, and `.RFS` format each have their own version numbers.
- Save migration:
  - Always scripted, with round-trip tests.
- Determinism is a **hard constraint**:
  - Any regression is treated as a bug, not a “feature”.

**End of Retrofactory Project Specification – Master v10**
