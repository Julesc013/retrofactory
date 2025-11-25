#include "present/sdl2/pres_sdl2.h"

namespace
{
    bool sdl_create(PresentHandle *handle, const PresentInitParams *params)
    {
        if (!present_simple_create(handle, params))
        {
            return false;
        }
        handle->caps.supports_linear_filter = 1;
        handle->caps.vsync_available = params ? params->vsync : 0;
        return true;
    }

    void sdl_destroy(PresentHandle *handle)
    {
        present_simple_destroy(handle);
    }

    void sdl_begin(PresentHandle *handle)
    {
        present_simple_begin(handle);
    }

    void sdl_end(PresentHandle *handle)
    {
        present_simple_end(handle);
    }

    void sdl_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
    {
        present_simple_noop_sprite(handle, cmd);
    }

    void sdl_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
    {
        present_simple_noop_quad(handle, cmd);
    }

    const PresentVTable kVTable = {
        sdl_create,
        sdl_destroy,
        sdl_begin,
        sdl_end,
        sdl_draw_sprite,
        sdl_draw_quad};
}

const PresentVTable *present_vtable_sdl2(void)
{
    return &kVTable;
}
