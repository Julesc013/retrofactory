#pragma once

#include "system/types.h"
#include "world/world.h"
#include "mods/prototypes.h"

/* Generate deterministic terrain for a world using provided RNG stream. */
bool worldgen_generate(World &world, u64 seed, const PrototypeStore *prototypes = 0);
