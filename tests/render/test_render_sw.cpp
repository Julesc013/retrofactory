#include "core/core.h"
#include "engine/snapshot.h"
#include "render/rend_sw.h"

int main()
{
    CoreConfig cfg;
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
    render_backbuffer_init(&buffer, 64u, 64u);
    RenderContext rc = make_render_context(&snapshot, &buffer);

    if (!rend_sw_init(0, 0))
    {
        render_backbuffer_free(&buffer);
        core_shutdown(state);
        return 1;
    }

    if (!rend_sw_frame(&rc, 0))
    {
        rend_sw_shutdown(0);
        render_backbuffer_free(&buffer);
        core_shutdown(state);
        return 1;
    }

    const u64 first = render_backbuffer_checksum(&buffer);
    if (!rend_sw_frame(&rc, 0))
    {
        rend_sw_shutdown(0);
        render_backbuffer_free(&buffer);
        core_shutdown(state);
        return 1;
    }
    const u64 second = render_backbuffer_checksum(&buffer);

    rend_sw_shutdown(0);
    render_backbuffer_free(&buffer);
    core_shutdown(state);
    return (first == 0u || first != second) ? 1 : 0;
}
