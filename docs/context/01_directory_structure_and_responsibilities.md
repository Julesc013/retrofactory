# Directory Structure and Responsibilities

This document explains, at a practical level, what each directory in the repo is for and how it should be used.

## Top-level Layout



Out-of-tree build artifacts (object files, intermediate binaries).

Must not be committed to version control.

Build system (CMake or equivalent) targets this directory.

config/

Default and edition-specific configuration files.

Examples:

core_default.cfg, video_default.cfg, audio_default.cfg, input_default.cfg.

edition_1x.cfg, edition_3x.cfg, edition_9x.cfg, edition_nx.cfg, edition_lx.cfg, edition_wx.cfg.

mods_default.cfg for initial mod set.

render_matrix.cfg for allowed render backends per edition/platform.

Runtime code in source/runtime and source/engine reads these.

data/

Game content, split into base content and planet/space packs.

Each pack has the same internal layout:

data/<pack>/
  audio/
    music/
    sounds/
  graphics/
    sprites/
    tiles/
    ui/
  locale/
    en/
  prototypes/
  scripts/


base/ – core game content (items, machines, recipes, tech, worldgen profiles, profiles.json).

mercury/, venus/, mars/, jupiter/, saturn/ – planet-specific content packs.

space/ – orbital/space infrastructure content pack.

Design:

Treat each pack as a mod-like content unit.

prototypes/ contains JSON-like prototype definitions.

scripts/ reserved for future Tier 1 script content.

locale/ contains strings for UI and content.

graphics/ and audio/ hold art and sound assets.

dist/

Distribution artefacts (installers, archives, disk images).

Populated by packaging scripts or CI pipelines.

Not source; safe to clean/regenerate.

docs/

Design and architecture documentation.

docs/context/ specifically contains the context files being generated here, used by code-generation tools.

external/

Vendored third-party libraries (SDL, Lua, PCG, zlib, etc.) when needed.

Only populated when local vendoring is required.

source/

All engine, simulation, and platform code.

Subdirectories are documented in 01_repo_layout_and_module_responsibilities.md.

tests/

Unit tests, determinism tests, perf tests, and integration tests.

Structured into subdirectories (core, determinism, perf, render, etc.) as test coverage grows.

tools/

Developer-facing tools: asset packers, sprite builders, save inspectors, mod SDK helpers.

Built as separate executables; not part of core runtime.