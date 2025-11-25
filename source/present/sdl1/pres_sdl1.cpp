#include "present/sdl1/pres_sdl1.h"

namespace
{
    bool sdl1_create(PresentHandle *handle, const PresentInitParams *params)
    {
        return present_simple_create(handle, params);
    }

    void sdl1_destroy(PresentHandle *handle)
    {
        present_simple_destroy(handle);
    }

    void sdl1_begin(PresentHandle *handle)
    {
        present_simple_begin(handle);
    }

    void sdl1_end(PresentHandle *handle)
    {
        present_simple_end(handle);
    }

    void sdl1_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
    {
        present_simple_noop_sprite(handle, cmd);
    }

    void sdl1_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
    {
        present_simple_noop_quad(handle, cmd);
    }

    const PresentVTable kVTable = {
        sdl1_create,
        sdl1_destroy,
        sdl1_begin,
        sdl1_end,
        sdl1_draw_sprite,
        sdl1_draw_quad};
}

const PresentVTable *present_vtable_sdl1(void)
{
    return &kVTable;
}
