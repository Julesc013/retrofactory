#include "engine/engine.h"
#include "engine/snapshot.h"
#include "present/software/sw_core.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    EngineContext engine;
    if (!engine_init(engine, 0))
    {
        return 1;
    }

    RenderContext rc = make_render_context(0, 0);
    rc.camera.width = 32u;
    rc.camera.height = 24u;

    u32 frame;
    for (frame = 0u; frame < 120u; ++frame)
    {
        SnapshotWorld snapshot;
        if (!engine_tick(engine) || !snapshot_build(engine.core_state, snapshot))
        {
            break;
        }
        rc.snapshot = &snapshot;
        sw_core_present(rc);
    }

    engine_shutdown(engine);
    return frame == 120u ? 0 : 1;
}
