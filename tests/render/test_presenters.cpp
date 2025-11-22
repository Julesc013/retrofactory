#include "core/core.h"
#include "engine/snapshot.h"
#include "present/dx5/pres_dx5.h"
#include "present/sdl2/pres_sdl2.h"
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
    render_backbuffer_init(buffer, 96u, 72u);
    RenderContext rc = make_render_context(&snapshot, &buffer);
    rc.camera.width = 32u;
    rc.camera.height = 24u;

    if (!rend_sw_init() || !rend_sw_frame(rc))
    {
        core_shutdown(state);
        render_backbuffer_free(buffer);
        rend_sw_shutdown();
        return 1;
    }
    const u64 sw_hash = render_backbuffer_checksum(buffer);

    if (!pres_dx5_init() || !pres_dx5_present(rc))
    {
        core_shutdown(state);
        render_backbuffer_free(buffer);
        rend_sw_shutdown();
        pres_dx5_shutdown();
        return 1;
    }
    const u64 dx_hash = render_backbuffer_checksum(buffer);

    if (!pres_sdl2_init() || !pres_sdl2_present(rc))
    {
        core_shutdown(state);
        render_backbuffer_free(buffer);
        rend_sw_shutdown();
        pres_dx5_shutdown();
        pres_sdl2_shutdown();
        return 1;
    }
    const u64 sdl_hash = render_backbuffer_checksum(buffer);

    rend_sw_shutdown();
    pres_dx5_shutdown();
    pres_sdl2_shutdown();
    render_backbuffer_free(buffer);
    core_shutdown(state);

    if (sw_hash == 0u || dx_hash == 0u || sdl_hash == 0u)
    {
        return 1;
    }
    return (sw_hash == dx_hash && dx_hash == sdl_hash) ? 0 : 1;
}
