#include "present/gdi/pres_gdi.h"
#include "present/present_backend.h"

static bool gdi_create(PresentHandle *handle, const PresentInitParams *params)
{
    return present_simple_create(handle, params);
}

static void gdi_destroy(PresentHandle *handle)
{
    present_simple_destroy(handle);
}

static void gdi_begin(PresentHandle *handle)
{
    present_simple_begin(handle);
}

static void gdi_end(PresentHandle *handle)
{
    present_simple_end(handle);
}

static void gdi_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
{
    present_simple_noop_sprite(handle, cmd);
}

static void gdi_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
{
    present_simple_noop_quad(handle, cmd);
}

static const PresentVTable kVTable = {
    gdi_create,
    gdi_destroy,
    gdi_begin,
    gdi_end,
    gdi_draw_sprite,
    gdi_draw_quad};

const PresentVTable *present_vtable_gdi(void)
{
    return &kVTable;
}

bool pres_gdi_init(void)
{
    return true;
}

bool pres_gdi_present(void)
{
    return true;
}

void pres_gdi_shutdown(void)
{
}
