# Mods, Content Packs, and Data Model

This document explains how `data/` and `source/mods/` cooperate.

## Content Packs under `data/`

Top-level structure:

```text
data/
  base/
  mercury/
  venus/
  mars/
  jupiter/
  saturn/
  space/
Each pack has:

text
Copy code
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
base/:

Core game items, machines, recipes, tech, worldgen, and profiles.

Planet packs (mercury, venus, mars, jupiter, saturn):

Planet-specific surfaces, scenarios, and content that rely on base definitions.

space/:

Space layer content: orbital platforms, rockets, interplanetary logistics.

Each pack should eventually include a manifest (e.g. manifest.json) describing:

ID, name, version, authors.

Dependencies (usually on base, and for planetary packs, possibly on space).

Supported editions and profiles.

Mod Layers under source/mods/
Subdirectories:

data/:

Tier 0 data mod loading logic.

Manifest parsing, dependency resolution, registry.

Application of data to core prototypes.

script/:

Tier 1 deterministic scripting system (future).

VM abstraction and core bindings.

binary/:

Tier 2 native/binary mod loader and ABI (future).

Core types and responsibilities:

mod_api:

C API exposed to script/binary mods.

Only stable, essential hooks.

mod_manifest:

Read and validate mod metadata (including content packs and user mods).

mod_loader:

Discover mods and content packs.

Apply Tier 0 data to prototype tables in core.

mod_depgraph:

Build dependency graphs and resolve order/conflicts.

mod_registry:

Store loaded mod descriptors and capabilities.

Mod Tiers
Tier 0 – Data mods

Data-only (prototypes, assets, locale).

Deterministic by design.

Supported everywhere.

Tier 1 – Script mods (future)

Built on a deterministic VM in source/mods/script.

Only enabled on capable platforms/profiles.

Disabled for deterministic MP and retro editions unless strictly controlled.

Tier 2 – Binary mods (future)

Native code modules loaded via binary loader.

Potentially non-deterministic and platform-specific.

Strictly opt-in, disabled by default for most editions.

Data Flow
Engine start:

Runtime resolves config and paths.

Mod loader discovers:

Base content pack.

Other packs under data/.

User mods.

Dependency resolution:

mod_depgraph orders content packs.

mod_registry records enabled packs.

Prototype assembly:

mod_loader builds final prototype tables from base + packs.

Core is initialised using these tables.

Simulation:

Core uses prototypes and instance data for all simulation logic.

No hard-coded knowledge about specific packs (e.g. planets) in core.

Rules for Codex
New content pack:

Belongs under data/<pack> with the same internal layout.

Requires manifest and prototypes; no code changes in core.

Mod-related logic:

Implement in source/mods/, not directly in core or world.

No ad-hoc conditionals in kernel code:

Avoid if (planet == Mars) or if (pack == space) in core/engine.

Route such distinctions through data, profiles, and mod APIs.

markdown
Copy code
