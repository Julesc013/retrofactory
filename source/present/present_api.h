#pragma once

#include "render/rend_api.h"
#include "render/rend_backend.h"
#include "render/rend_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct PresentHandle;
struct PresentInitParams;
typedef struct PresentVTable
{
    bool (*create)(struct PresentHandle *handle, const struct PresentInitParams *params);
    void (*destroy)(struct PresentHandle *handle);
    void (*begin_frame)(struct PresentHandle *handle);
    void (*end_frame)(struct PresentHandle *handle);
    void (*draw_sprite)(struct PresentHandle *handle, const SpriteDrawCmd *cmd);
    void (*draw_quad)(struct PresentHandle *handle, const QuadDrawCmd *cmd);
} PresentVTable;

typedef struct PresentHandle
{
    RenderBackendId backend;
    RenderBackendCaps caps;
    RenderBackbuffer backbuffer;
    void *user_data;
    const struct PresentVTable *vtable;
    u64 frame_count;
    u64 last_checksum;
} PresentHandle;

typedef struct PresentInitParams
{
    RenderBackendId backend;
    int width;
    int height;
    int fullscreen;
    int vsync;
    void *native_window;
} PresentInitParams;

PresentHandle *present_create(const PresentInitParams *params);
void present_destroy(PresentHandle *handle);

void present_get_caps(PresentHandle *handle, RenderBackendCaps *caps);
RenderBackbuffer *present_backbuffer(PresentHandle *handle);

void present_begin_frame(PresentHandle *handle);
void present_end_frame(PresentHandle *handle);

void present_draw_sprite(PresentHandle *handle, const SpriteDrawCmd *cmd);
void present_draw_quad(PresentHandle *handle, const QuadDrawCmd *cmd);

u64 present_frame_count(PresentHandle *handle);
u64 present_last_checksum(PresentHandle *handle);

#ifdef __cplusplus
}
#endif
