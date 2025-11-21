#pragma once

#include "system/types.h"
#include "world/world.h"

/* Generate deterministic terrain for a world using provided RNG stream. */
bool worldgen_generate(World &world, u64 seed);
