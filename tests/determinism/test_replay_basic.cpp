#include "core/core.h"
#include "engine/snapshot.h"
#include "engine/replay.h"

int main()
{
    const u32 frames = 8u;
    const u32 ticks_per_frame = 5u;

    CoreConfig cfg;
    cfg.initial_seed = 9001u;

    CoreState a;
    CoreState b;
    if (!core_init(a, cfg) || !core_init(b, cfg))
    {
        return 1;
    }

    ReplayFrame frames_a[8];
    ReplayFrame frames_b[8];

    u32 i, t;
    for (i = 0u; i < frames; ++i)
    {
        for (t = 0u; t < ticks_per_frame; ++t)
        {
            core_tick(a);
            core_tick(b);
        }

        SnapshotWorld snap_a;
        SnapshotWorld snap_b;
        snapshot_build(a, snap_a);
        snapshot_build(b, snap_b);
        replay_record_frame(snap_a, frames_a[i]);
        replay_record_frame(snap_b, frames_b[i]);
    }

    for (i = 0u; i < frames; ++i)
    {
        if (frames_a[i].tick != frames_b[i].tick ||
            frames_a[i].hash != frames_b[i].hash ||
            frames_a[i].hash == 0u)
        {
            core_shutdown(a);
            core_shutdown(b);
            return 1;
        }
    }

    core_shutdown(a);
    core_shutdown(b);
    return 0;
}
