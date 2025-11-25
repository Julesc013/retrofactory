#include "present/web/pres_web.h"

namespace
{
    bool web_create(PresentHandle *handle, const PresentInitParams *params)
    {
        return present_simple_create(handle, params);
    }

    void web_destroy(PresentHandle *handle)
    {
        present_simple_destroy(handle);
    }

    void web_begin(PresentHandle *handle)
    {
        present_simple_begin(handle);
    }

    void web_end(PresentHandle *handle)
    {
        present_simple_end(handle);
    }

    void web_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
    {
        present_simple_noop_sprite(handle, cmd);
    }

    void web_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
    {
        present_simple_noop_quad(handle, cmd);
    }

    const PresentVTable kVTable = {
        web_create,
        web_destroy,
        web_begin,
        web_end,
        web_draw_sprite,
        web_draw_quad};
}

const PresentVTable *present_vtable_web(void)
{
    return &kVTable;
}
