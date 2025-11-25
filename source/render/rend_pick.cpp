#include "render/rend_pick.h"

#include "render/rend_dx.h"
#include "render/rend_gl.h"
#include "render/rend_metal.h"
#include "render/rend_sdl.h"
#include "render/rend_sw.h"
#include "render/rend_vk.h"

namespace
{
    RenderAdapter make_stub_adapter()
    {
        RenderAdapter adapter;
        adapter.init = 0;
        adapter.frame = 0;
        adapter.shutdown = 0;
        return adapter;
    }
}

const RenderAdapter *rend_pick_adapter(const RenderBackendId &backend)
{
    switch (backend.family)
    {
    case REND_FAMILY_SOFTWARE:
        return rend_sw_adapter();
    case REND_FAMILY_SDL:
        return rend_sdl_adapter();
    case REND_FAMILY_GDI:
    case REND_FAMILY_QDRAW:
    case REND_FAMILY_WEB:
        return rend_sw_adapter();
    case REND_FAMILY_DX:
        return rend_dx_adapter();
    case REND_FAMILY_GL:
        return rend_gl_adapter();
    case REND_FAMILY_VK:
        return rend_vk_adapter();
    case REND_FAMILY_METAL:
        return rend_metal_adapter();
    default:
        break;
    }
    static RenderAdapter stub = make_stub_adapter();
    return &stub;
}
