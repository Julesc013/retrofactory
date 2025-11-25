#include "present/present_api.h"
#include "present/present_backend.h"

#include <new>

namespace
{
    const u32 kDefaultWidth = 320u;
    const u32 kDefaultHeight = 200u;

    void fill_default_caps(RenderBackendCaps &caps)
    {
        caps.max_width = 1920;
        caps.max_height = 1080;
        caps.max_texture_size = 1024;
        caps.supports_linear_filter = 0;
        caps.supports_aniso = 0;
        caps.supports_shader_effects = 0;
        caps.prefers_power_of_two = 1;
        caps.vsync_available = 0;
        caps.float_render_targets = 0;
    }

    bool ensure_buffer(PresentHandle *handle, int width, int height)
    {
        const u32 w = width > 0 ? static_cast<u32>(width) : kDefaultWidth;
        const u32 h = height > 0 ? static_cast<u32>(height) : kDefaultHeight;
        if (handle->backbuffer.width == w && handle->backbuffer.height == h && handle->backbuffer.pixels != 0)
        {
            return true;
        }
        render_backbuffer_free(&handle->backbuffer);
        return render_backbuffer_init(&handle->backbuffer, w, h);
    }

    void clear_buffer(PresentHandle *handle, u32 color)
    {
        if (handle == 0 || handle->backbuffer.pixels == 0)
        {
            return;
        }
        const u32 total = handle->backbuffer.width * handle->backbuffer.height;
        u32 i;
        for (i = 0u; i < total; ++i)
        {
            handle->backbuffer.pixels[i] = color;
        }
    }

    const PresentVTable *pick_vtable(const RenderBackendId &id)
    {
        switch (id.family)
        {
        case REND_FAMILY_SOFTWARE:
            return present_vtable_software();
        case REND_FAMILY_SDL:
            if (id.version == REND_VER_SDL1)
            {
                return present_vtable_sdl1();
            }
            if (id.version == REND_VER_SDL2)
            {
                return present_vtable_sdl2();
            }
            return 0;
        case REND_FAMILY_GDI:
            return present_vtable_gdi();
        case REND_FAMILY_QDRAW:
            return present_vtable_qdraw();
        case REND_FAMILY_WEB:
            return present_vtable_web();
        case REND_FAMILY_DX:
        case REND_FAMILY_GL:
        case REND_FAMILY_VK:
        case REND_FAMILY_METAL:
        default:
            return present_vtable_dx_stub();
        }
    }
}

#ifdef __cplusplus
extern "C" {
#endif

PresentHandle *present_create(const PresentInitParams *params)
{
    if (params == 0)
    {
        return 0;
    }
    const PresentVTable *vtable = pick_vtable(params->backend);
    if (vtable == 0)
    {
        return 0;
    }

    PresentHandle *handle = new (std::nothrow) PresentHandle();
    if (handle == 0)
    {
        return 0;
    }
    handle->backend = params->backend;
    fill_default_caps(handle->caps);
    handle->backbuffer.width = 0u;
    handle->backbuffer.height = 0u;
    handle->backbuffer.pixels = 0;
    handle->backbuffer.size = 0u;
    handle->backbuffer.capacity = 0u;
    handle->user_data = 0;
    handle->vtable = vtable;
    handle->frame_count = 0u;
    handle->last_checksum = 0u;

    if (!vtable->create(handle, params))
    {
        delete handle;
        return 0;
    }
    return handle;
}

void present_destroy(PresentHandle *handle)
{
    if (handle == 0)
    {
        return;
    }
    if (handle->vtable != 0 && handle->vtable->destroy != 0)
    {
        handle->vtable->destroy(handle);
    }
    delete handle;
}

void present_get_caps(PresentHandle *handle, RenderBackendCaps *caps)
{
    if (handle == 0 || caps == 0)
    {
        return;
    }
    *caps = handle->caps;
}

RenderBackbuffer *present_backbuffer(PresentHandle *handle)
{
    if (handle == 0)
    {
        return 0;
    }
    return &handle->backbuffer;
}

void present_begin_frame(PresentHandle *handle)
{
    if (handle == 0 || handle->vtable == 0 || handle->vtable->begin_frame == 0)
    {
        return;
    }
    handle->vtable->begin_frame(handle);
}

void present_end_frame(PresentHandle *handle)
{
    if (handle == 0 || handle->vtable == 0 || handle->vtable->end_frame == 0)
    {
        return;
    }
    handle->vtable->end_frame(handle);
}

void present_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
{
    if (handle == 0 || handle->vtable == 0 || handle->vtable->draw_sprite == 0)
    {
        return;
    }
    handle->vtable->draw_sprite(handle, cmd);
}

void present_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
{
    if (handle == 0 || handle->vtable == 0 || handle->vtable->draw_quad == 0)
    {
        return;
    }
    handle->vtable->draw_quad(handle, cmd);
}

u64 present_frame_count(PresentHandle *handle)
{
    return handle ? handle->frame_count : 0u;
}

u64 present_last_checksum(PresentHandle *handle)
{
    return handle ? handle->last_checksum : 0u;
}

/* --------------------------------------------------------------------------
 * Shared helpers for simple backends (software, SDL, GDI, QDRAW, WEB).
 * These are exposed so backend .cpp files can reuse the same allocation path.
 * -------------------------------------------------------------------------- */

bool present_simple_create(PresentHandle *handle, const PresentInitParams *params)
{
    if (handle == 0 || params == 0)
    {
        return false;
    }
    fill_default_caps(handle->caps);
    return ensure_buffer(handle, params->width, params->height);
}

void present_simple_destroy(PresentHandle *handle)
{
    if (handle == 0)
    {
        return;
    }
    render_backbuffer_free(&handle->backbuffer);
}

void present_simple_begin(PresentHandle *handle)
{
    if (handle == 0)
    {
        return;
    }
    /* Simple presenter just clears to transparent black. */
    clear_buffer(handle, 0u);
}

void present_simple_end(PresentHandle *handle)
{
    if (handle == 0)
    {
        return;
    }
    handle->last_checksum = render_backbuffer_checksum(&handle->backbuffer);
    handle->frame_count += 1u;
}

void present_simple_noop_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
{
    (void)handle;
    (void)cmd;
}

void present_simple_noop_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
{
    (void)handle;
    (void)cmd;
}

#ifdef __cplusplus
} /* extern "C" */
#endif
