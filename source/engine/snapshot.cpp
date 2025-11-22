#include "engine/snapshot.h"

bool snapshot_build(const CoreState &state, SnapshotWorld &out_snapshot)
{
    out_snapshot.world = &state.world;
    out_snapshot.entities = &state.entities;
    out_snapshot.networks = &state.networks;
    out_snapshot.research = &state.research;
    out_snapshot.prototypes = state.prototypes;
    out_snapshot.tick = state.tick;
    return true;
}
