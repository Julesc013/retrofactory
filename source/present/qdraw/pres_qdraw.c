#include "present/qdraw/pres_qdraw.h"
#include "present/present_backend.h"

static bool qd_create(PresentHandle *handle, const PresentInitParams *params)
{
    return present_simple_create(handle, params);
}

static void qd_destroy(PresentHandle *handle)
{
    present_simple_destroy(handle);
}

static void qd_begin(PresentHandle *handle)
{
    present_simple_begin(handle);
}

static void qd_end(PresentHandle *handle)
{
    present_simple_end(handle);
}

static void qd_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
{
    present_simple_noop_sprite(handle, cmd);
}

static void qd_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
{
    present_simple_noop_quad(handle, cmd);
}

static const PresentVTable kVTable = {
    qd_create,
    qd_destroy,
    qd_begin,
    qd_end,
    qd_draw_sprite,
    qd_draw_quad};

const PresentVTable *present_vtable_qdraw(void)
{
    return &kVTable;
}

bool pres_qdraw_init(void)
{
    return true;
}

bool pres_qdraw_present(void)
{
    return true;
}

void pres_qdraw_shutdown(void)
{
}
