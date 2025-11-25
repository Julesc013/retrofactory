# Editions, Profiles, and Platforms

This document captures how editions map onto platform code and performance profiles.

## Editions

Conceptual edition IDs:

- **1x** – MS-DOS (DOS 3–8).
- **3x** – Windows 3.x.
- **9x** – Windows 9x family.
- **Nx** – Windows NT family.
- **Lx** – POSIX desktop (Linux/BSD).
- **Wx** – WebAssembly/WebGL (browser).
- Others (classic Mac, Android) are future variants.

Each edition is described by:

- An edition config file under `config/`.
- A subset of supported platforms/renderers.
- A set of allowed profiles and content packs.

## Platform Mapping

`source/platform/` contains per-platform code:

- `msdos/` – used for 1x edition.
- `win16/` – used for 3x edition.
- `win32/` – used for 9x and Nx editions (with different config).
- `posix/` – used for Lx edition.
- `web/` – used for Wx edition.

Common interface:

- `plat_api.h` – abstract API for:
  - Window/surface creation and events.
  - Input polling.
  - Audio playback.
  - Filesystem access.
  - Time/tick/sleep functions.
  - Logging sinks.
- `plat_ids.h` – platform IDs and optional mapping to edition IDs.

Each `<platform>_main.cpp`:

- Contains the entrypoint (`main` or `WinMain`).
- Bootstraps platform-specific context.
- Invokes engine/launcher entrypoints from `source/engine` / `source/interface`.

## Profiles

Profiles are defined in data (`data/base/prototypes/`) and mirrored in config:

- Data-driven fields:
  - Max colonies, entities, chunks, networks.
  - Algorithm variants (simple, advanced).
  - Feature toggles.
- Config-driven fields:
  - Selected profile per edition (`config/edition_*.cfg`).
  - Allowed combinations for content packs.

`source/config/profile.*`:

- Maps profile IDs to:
  - Caps.
  - Algorithm tier choices.
  - Enabled/disabled features (e.g. advanced fluid network).

Engine usage:

- Engine reads:
  - `config/edition_*.cfg`.
  - Profile definitions from `data/base/prototypes`.
- Constructs an effective runtime profile object used by core/world modules.

## Render Matrix

`config/render_matrix.cfg`:

- Describes which render backends are allowed per edition/platform combination.
- Example rules:
  - 1x (msdos): software renderer only (`rnd_sw`) at fixed resolutions.
  - 3x (win16): software renderer via GDI or equivalent.
  - 9x (win32): software + `rnd_dx5` (and optionally `rnd_gl1`).
  - Nx (win32): software + `rnd_gl1` + `rnd_gl2` depending on hardware.
  - Lx (posix): software + `rnd_gl1` + `rnd_gl2`.
  - Wx (web): GL-like backend via WebGL.

Renderer selection:

- `source/render/rnd_pick.*` reads:
  - Effective profile.
  - Render matrix.
  - Platform capabilities via `plat_api`.
- Chooses a backend and configures it for engine/interface.

## Rules for Core Code

- Core and world modules:
  - Must not directly check edition or platform.
  - Only see profiles and generic capabilities.

- Edition- or platform-specific behaviour:
  - Expressed via profiles, config files, and platform/render modules.
  - Not embedded as ad-hoc checks in core simulation code.
