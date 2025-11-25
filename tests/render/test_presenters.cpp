#include "core/core.h"
#include "engine/snapshot.h"
#include "render/rend_sw.h"

int main()
{
    CoreConfig cfg;
    cfg.initial_seed = 77u;
    CoreState state;
    if (!core_init(state, cfg))
    {
        return 1;
    }

    SnapshotWorld snapshot;
    if (!snapshot_build(state, snapshot))
    {
        core_shutdown(state);
        return 1;
    }

    RenderBackbuffer buffer;
    render_backbuffer_init(&buffer, 96u, 72u);
    RenderContext rc = make_render_context(&snapshot, &buffer);
    rc.camera.width = 32u;
    rc.camera.height = 24u;

    if (!rend_sw_init(0, 0) || !rend_sw_frame(&rc, 0))
    {
        core_shutdown(state);
        render_backbuffer_free(&buffer);
        rend_sw_shutdown(0);
        return 1;
    }
    const u64 sw_hash = render_backbuffer_checksum(&buffer);

    rend_sw_shutdown(0);
    render_backbuffer_free(&buffer);
    core_shutdown(state);

    if (sw_hash == 0u)
    {
        return 1;
    }
    return 0;
}
