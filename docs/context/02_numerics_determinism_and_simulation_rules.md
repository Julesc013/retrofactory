# Numerics, Determinism, and Simulation Rules

This document defines how numeric types, random numbers, and tick ordering must be implemented to keep the simulation deterministic.

## Numeric Model

Base units (conceptual):

- Time: ticks (e.g. 60 ticks/sec).
- Position: sub-tile units (integer grid with fixed-point offsets).
- Mass: grams.
- Volume: millilitres.
- Energy: Joules or Joules-per-tick.
- Temperature: milliKelvin.

Suggested types:

- Tile coordinates: `int32_t`.
- Sub-tile positions: fixed-point represented as `int32_t` or `int64_t`.
- Item counts: `int32_t`.
- Fluid volumes: `int32_t`.
- Energy: `int32_t` local, `int64_t` for global accumulators.
- Temperature: `int32_t` (mK).
- Tick counter: `uint64_t`.

Rules:

- No floating-point arithmetic in simulation state updates.
- Deterministic behaviour on overflow:
  - If a quantity can overflow, clamp or wrap in a well-defined way.
- No reliance on implementation-defined behaviour (shifts, signed overflow, etc.).

## RNG Rules

- All random operations must go through `source/system/rng.h`.
- RNG features:
  - Deterministic sequence given a seed.
  - Serialisable state (for save/load and replays).
  - Support for multiple RNG streams (worldgen, macro AI, etc.).
- Forbidden:
  - `rand()` from `<cstdlib>`.
  - `<random>` facilities.
  - Platform-specific randomness APIs.

## Tick Scheduling

- `source/schedule/tick.*` defines canonical tick phases and ordering.
- Example phases (names are illustrative):
  - `TICK_PHASE_INPUT`
  - `TICK_PHASE_LOGIC`
  - `TICK_PHASE_NETWORKS`
  - `TICK_PHASE_RESEARCH`
  - `TICK_PHASE_CLEANUP`
- All systems that update simulation state must be wired through these phases.
- No out-of-band updates that circumvent the scheduler.

## Event System

- `source/schedule/events.*` provides event queues.
- Events must be:
  - Local to the tick (or explicitly delayed to future tick).
  - Delivered in deterministic order.
- Use events to decouple systems (e.g. logistic events into network code) without breaking determinism.

## Save/Load and State Hashing

- `source/save/save.*`:
  - Versioned `.RFS` save format with explicit layout and endianness.
  - Kernels must not assume host endianness or struct packing.
- `source/save/state_hash.*`:
  - Computes stable hashes across:
    - Tick counter.
    - RNG state.
    - World state.
    - Core system state (networks, research, etc.).
  - Used for:
    - Replay verification.
    - Cross-platform determinism tests.

Requirements:

- After save + load:
  - Core state, world state, RNG, and hash must match.
- Replays:
  - Running the same input sequence must produce identical hashes at known checkpoints.

## Profiles and Algorithm Tiers

Profiles (defined in data and reflected in `source/config/profile.*`) decide:

- Caps:
  - Max entities, chunks, networks, etc.
- Algorithm tiers:
  - Simple vs advanced network algorithms, heuristics, or approximations.

Core rules:

- Profile selection must not break determinism.
- Differences between profiles:
  - Change complexity and capacity, not correctness.
- All profile-driven decisions must be data-driven; no hard-coded edition checks in simulation code.

## Macro Layer and Transit

- Transit systems are modelled deterministically:
  - Manifests and schedules are computed with fixed rules.
  - Arrival times and flows are not perturbed with noise unless derived from RNG streams that are properly serialised.
- Macro AI must behave deterministically given the same inputs.

Any code generation that influences simulation must respect all of the above rules.
