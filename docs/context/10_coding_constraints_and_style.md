

---

### `docs/context/05_coding_rules_requirements_and_prohibitions.md`

```markdown
# Coding Rules, Requirements, and Prohibitions

This document consolidates hard rules. It extends the style notes in `02_coding_constraints_and_style.md`.

## Absolutely Required

1. **Determinism in Kernel**
   - All simulation state updates in:
     - `source/core`
     - `source/world`
     - `source/schedule`
     - `source/save`
   must be deterministic given the same inputs, config, and content.

2. **C++98 Subset for Kernel**
   - Kernel modules must compile as C++98:
     - No exceptions.
     - No RTTI.
     - No C++11+ features.

3. **Centralised RNG**
   - All random behaviour uses `source/system/rng.*`.
   - RNG state must be serialisable and part of save/load.

4. **Strict Module Dependencies**
   - Kernel modules must not include platform headers.
   - Render must not depend on core internals beyond public APIs.
   - Platform must not depend directly on core internals, only on engine/interface entrypoints.

5. **Save and Hash**
   - Save/load must preserve state exactly for core/world.
   - `state_hash` must be stable across platforms and compilers.

6. **Tests for Simulation Changes**
   - Any change to simulation logic, numeric types, or profiles must have accompanying tests in `tests/`.

## Forbidden in Kernel

- Exceptions: `throw`, `try`, `catch`.
- RTTI: `dynamic_cast`, `typeid`.
- Heavy STL in hot paths:
  - `std::map`, `std::unordered_*`, `std::list` and similar.
- Platform or graphics includes (Windows, POSIX, SDL, OpenGL, DirectX, Emscripten).
- Using wall-clock time, threads, or external I/O in simulation paths.
- Floating point arithmetic in simulation state updates.

## Strongly Discouraged

- Over-fragmenting modules into many tiny files.
- Global mutable state that is not clearly owned or serialised.
- Hidden dependencies between modules (e.g. core reaching into engine internals).

## Permitted with Caution

- STL in non-hot paths (e.g. runtime config parsing, mod discovery).
- Templates in `source/utility` where they simplify containers or math.
- More modern C++ features and STL in:
  - `source/platform`
  - `tools/`
  - `tests/`
as long as they are kept out of kernel interfaces.

## File/Module Growth Strategy

- Prefer to extend existing modules rather than creating new directories.
  - New logistics behaviour → `source/core/logistic.*`.
  - New energy detail → `source/core/energy.*`.
  - New profile field → `source/config/profile.*`.
- Only split large modules when:
  - Single file is clearly too large and internally separable.
  - Split respects existing dependencies and layering.

## Edition and Platform Logic

- Forbidden:
  - `if (edition == 1x)` or `if (platform == msdos)` checks in kernel code.
- Allowed:
  - Data-driven differences using profiles and content.
  - Platform-specific behaviour in `source/platform`.
  - Render differences in `source/render`.

## Behaviour for Code Generation

Any automated code generation must:

- Use the directory/module mapping from `01_repo_layout_and_module_responsibilities.md`.
- Place new code in the minimal correct module.
- Not introduce new top-level modules or directories without explicit design.
- Check whether a feature belongs in:
  - `core/world/schedule/save` (simulation),
  - `runtime` (config, logging, save slots),
  - `render` (drawing),
  - `interface` (UI, networking),
  - `platform` (OS-specific behaviour),
  - `tools` or `tests`.

Violations of any of these rules are considered design bugs.
