#pragma once

#include "core/core.h"

struct SnapshotWorld
{
    const World *world;
    const EntitiesState *entities;
    const NetworkState *networks;
    const ResearchState *research;
    const PrototypeStore *prototypes;
    Tick tick;
};

bool snapshot_build(const CoreState &state, SnapshotWorld &out_snapshot);
