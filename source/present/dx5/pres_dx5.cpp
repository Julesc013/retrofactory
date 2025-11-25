#include "present/dx5/pres_dx5.h"

namespace
{
    bool dx_stub_create(PresentHandle *handle, const PresentInitParams *params)
    {
        (void)handle;
        (void)params;
        return false; /* not implemented yet */
    }

    void dx_stub_destroy(PresentHandle *handle)
    {
        (void)handle;
    }

    void dx_stub_begin(PresentHandle *handle)
    {
        (void)handle;
    }

    void dx_stub_end(PresentHandle *handle)
    {
        (void)handle;
    }

    void dx_stub_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
    {
        (void)handle;
        (void)cmd;
    }

    void dx_stub_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
    {
        (void)handle;
        (void)cmd;
    }

    const PresentVTable kVTable = {
        dx_stub_create,
        dx_stub_destroy,
        dx_stub_begin,
        dx_stub_end,
        dx_stub_sprite,
        dx_stub_quad};
}

const PresentVTable *present_vtable_dx_stub(void)
{
    return &kVTable;
}
