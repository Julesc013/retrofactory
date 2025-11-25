#include "core/core.h"
#include "engine/snapshot.h"
#include "engine/replay.h"

int main()
{
    CoreConfig cfg_a;
    CoreConfig cfg_b;
    cfg_a.initial_seed = 1u;
    cfg_b.initial_seed = 2u;

    CoreState a;
    CoreState b;
    if (!core_init(a, cfg_a) || !core_init(b, cfg_b))
    {
        return 1;
    }

    u32 i;
    for (i = 0u; i < 20u; ++i)
    {
        core_tick(a);
        core_tick(b);
    }

    SnapshotWorld snap_a;
    SnapshotWorld snap_b;
    ReplayFrame frame_a;
    ReplayFrame frame_b;
    snapshot_build(a, snap_a);
    snapshot_build(b, snap_b);
    replay_record_frame(snap_a, frame_a);
    replay_record_frame(snap_b, frame_b);

    core_shutdown(a);
    core_shutdown(b);

    return (frame_a.hash == frame_b.hash) ? 1 : 0;
}
