#pragma once

#include "present/present_api.h"

#ifdef __cplusplus
extern "C" {
#endif

const PresentVTable *present_vtable_software(void);
const PresentVTable *present_vtable_sdl1(void);
const PresentVTable *present_vtable_sdl2(void);
const PresentVTable *present_vtable_dx_stub(void);
const PresentVTable *present_vtable_gdi(void);
const PresentVTable *present_vtable_qdraw(void);
const PresentVTable *present_vtable_web(void);

bool present_simple_create(PresentHandle *handle, const PresentInitParams *params);
void present_simple_destroy(PresentHandle *handle);
void present_simple_begin(PresentHandle *handle);
void present_simple_end(PresentHandle *handle);
void present_simple_noop_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd);
void present_simple_noop_quad(PresentHandle *handle, const QuadDrawCmd *cmd);

#ifdef __cplusplus
}
#endif
