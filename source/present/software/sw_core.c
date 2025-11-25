#include "present/software/sw_core.h"
#include "present/present_backend.h"

static bool sw_create(PresentHandle *handle, const PresentInitParams *params)
{
    return present_simple_create(handle, params);
}

static void sw_destroy(PresentHandle *handle)
{
    present_simple_destroy(handle);
}

static void sw_begin(PresentHandle *handle)
{
    present_simple_begin(handle);
}

static void sw_end(PresentHandle *handle)
{
    present_simple_end(handle);
}

static void sw_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd)
{
    present_simple_noop_sprite(handle, cmd);
}

static void sw_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd)
{
    present_simple_noop_quad(handle, cmd);
}

static const PresentVTable kVTable = {
    sw_create,
    sw_destroy,
    sw_begin,
    sw_end,
    sw_draw_sprite,
    sw_draw_quad};

const PresentVTable *present_vtable_software(void)
{
    return &kVTable;
}

bool sw_core_init(void)
{
    return true;
}

bool sw_core_present(void)
{
    return true;
}

void sw_core_shutdown(void)
{
}
