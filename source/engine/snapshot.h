#pragma once

#include "core/core.h"

struct SnapshotWorld
{
    const World *world;
    Tick tick;
};

bool snapshot_build(const CoreState &state, SnapshotWorld &out_snapshot);
