#include "engine/replay.h"

#include "saveload/hash.h"

bool replay_record_frame(const SnapshotWorld &snapshot, ReplayFrame &out_frame)
{
    if (snapshot.world == 0)
    {
        return false;
    }
    out_frame.tick = snapshot.tick;
    CoreState fake_state;
    fake_state.tick = snapshot.tick;
    fake_state.world = *snapshot.world;
    fake_state.rng.state = 0u;
    fake_state.rng.inc = 0u;
    out_frame.hash = compute_state_hash(fake_state);
    return true;
}
