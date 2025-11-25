#include "engine/engine.h"
#include "engine/replay.h"
#include "engine/snapshot.h"

int main()
{
    EngineContext ctx_a;
    EngineContext ctx_b;
    if (!engine_init(ctx_a, "config/edition_nx.cfg"))
    {
        return 1;
    }
    if (!engine_init(ctx_b, "config/edition_wx.cfg"))
    {
        engine_shutdown(ctx_a);
        return 1;
    }

    const u32 frames = 6u;
    u32 i;
    for (i = 0u; i < frames; ++i)
    {
        if (!engine_tick(ctx_a) || !engine_tick(ctx_b))
        {
            engine_shutdown(ctx_a);
            engine_shutdown(ctx_b);
            return 1;
        }
    }

    SnapshotWorld snap_a;
    SnapshotWorld snap_b;
    if (!snapshot_build(ctx_a.core_state, snap_a) || !snapshot_build(ctx_b.core_state, snap_b))
    {
        engine_shutdown(ctx_a);
        engine_shutdown(ctx_b);
        return 1;
    }

    ReplayFrame frame_a;
    ReplayFrame frame_b;
    if (!replay_record_frame(snap_a, frame_a) || !replay_record_frame(snap_b, frame_b))
    {
        engine_shutdown(ctx_a);
        engine_shutdown(ctx_b);
        return 1;
    }

    engine_shutdown(ctx_a);
    engine_shutdown(ctx_b);

    if (frame_a.hash == 0u || frame_b.hash == 0u)
    {
        return 1;
    }
    return (frame_a.hash == frame_b.hash) ? 0 : 1;
}
