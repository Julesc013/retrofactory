#include "engine/engine.h"
#include "engine/snapshot.h"
#include "present/dx5/pres_dx5.h"

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
    rc.camera.width = 48u;
    rc.camera.height = 36u;

    if (!pres_dx5_init())
    {
        engine_shutdown(engine);
        return 1;
    }

    u32 frame;
    for (frame = 0u; frame < 120u; ++frame)
    {
        SnapshotWorld snapshot;
        if (!engine_tick(engine) || !snapshot_build(engine.core_state, snapshot))
        {
            break;
        }
        rc.snapshot = &snapshot;
        if (!pres_dx5_present(rc))
        {
            break;
        }
    }

    pres_dx5_shutdown();
    engine_shutdown(engine);
    return frame == 120u ? 0 : 1;
}
