# Retrofactory – Project Vision and Scope

## Vision

Retrofactory is a long-lived, archival-friendly factory and colony simulation built to:

- Outlive individual hardware generations.
- Run on retro and modern systems with a unified ruleset.
- Support deep modding and multiple thematic content packs.
- Serve as an experimental platform for deterministic simulation design.

## Functional Scope

- **Core gameplay**
  - Resource extraction, processing, and logistics.
  - Power, fluids, heat, and other networks.
  - Worldgen for multiple planets and environments.
  - Colonies composed of tiles, chunks, and regions.
  - Research and tech progression.

- **Macro layer**
  - Planet-specific content packs (Mercury, Venus, Mars, Jupiter, Saturn).
  - Space layer content pack (`data/space`) for orbital infrastructure and inter-planetary links.
  - Aggregate transit and trade between colonies and planets.

- **Moddability**
  - Data-driven prototype definitions in JSON-like formats.
  - Tier 0 (data-only) mods from day one.
  - Tier 1 (script) and Tier 2 (binary/native) mods in later phases and on capable platforms.

## Non-Functional Scope

- **Determinism guarantee**
  - Reproducible simulation across platforms and compilers.
  - Lockstep multiplayer enabled by deterministic core.
  - Cross-platform replay and state-hash comparison.

- **Portability**
  - Edition set targets:
    - 1x: MS-DOS (via `source/platform/msdos`).
    - 3x: Windows 3.x (`source/platform/win16`).
    - 9x: Windows 9x family (`source/platform/win32`).
    - Nx: Windows NT family (`source/platform/win32`).
    - Lx: POSIX desktop (`source/platform/posix`).
    - Wx: WebAssembly/WebGL (`source/platform/web`).
    - Other editions (classic Mac, Android) are conceptual for now.

- **Architecture discipline**
  - Clear dependency direction:
    - system → core/world/schedule/save → engine/mods/runtime → render/interface → platform.
  - No reverse dependency from platform or render into core internals.
  - Configuration, profiles, and edition differences expressed via data/config, not hard-coded in core.

## Out of Scope for Now

- Advanced 3D rendering or physics.
- Complex scripting VMs and native mod systems in early phases.
- Cross-edition cloud features or services.
- Any feature that would compromise determinism or portable build.

This scope can expand, but only if compatible with the current layering and constraints.
