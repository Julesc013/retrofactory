#include "engine/engine.h"
#include "engine/snapshot.h"
#include "render/rend_sw.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    EngineContext engine;
    if (!engine_init(engine, 0))
    {
        return 1;
    }

    RenderContext rc;
    RenderBackbuffer buffer;
    render_backbuffer_init(buffer, 160u, 120u);
    rc.target = &buffer;
    rc.snapshot = 0;

    u32 frame;
    for (frame = 0u; frame < 120u; ++frame)
    {
        engine_tick(engine);
        SnapshotWorld snapshot;
        if (!snapshot_build(engine.core_state, snapshot))
        {
            render_backbuffer_free(buffer);
            engine_shutdown(engine);
            return 1;
        }
        rc.snapshot = &snapshot;
        rend_sw_frame(rc);
    }

    render_backbuffer_free(buffer);
    engine_shutdown(engine);
    return 0;
}
