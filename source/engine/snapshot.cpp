#include "engine/snapshot.h"

bool snapshot_build(const CoreState &state, SnapshotWorld &out_snapshot)
{
    out_snapshot.world = &state.world;
    out_snapshot.tick = state.tick;
    return true;
}
