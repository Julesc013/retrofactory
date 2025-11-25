#ifndef REND_API_H
#define REND_API_H

#include "system/types.h"
#include "render/rend_backend.h"
#include "render/rend_types.h"

struct SnapshotWorld;

typedef struct RenderBackbuffer
{
    u32 width;
    u32 height;
    u32 size;      /* number of pixels */
    u32 capacity;  /* allocated pixels */
    u32 *pixels;
} RenderBackbuffer;

typedef struct RenderCamera
{
    i32 origin_x;
    i32 origin_y;
    u32 width;
    u32 height;
} RenderCamera;

typedef struct RenderContext
{
    const struct SnapshotWorld *snapshot;
    RenderBackbuffer *target;
    RenderCamera camera;
} RenderContext;

typedef struct RenderInitParams
{
    RenderBackendId backend;
    int width;
    int height;
    int fullscreen;
    int vsync;
    void *native_window;
} RenderInitParams;

bool render_backbuffer_init(RenderBackbuffer *buffer, u32 width, u32 height);
void render_backbuffer_free(RenderBackbuffer *buffer);
u64 render_backbuffer_checksum(const RenderBackbuffer *buffer);

bool render_frame(RenderContext *ctx);
bool render_init(const RenderInitParams *params);
void render_shutdown(void);
bool render_draw(RenderContext *ctx);
RenderBackendId render_active_backend(void);
const RenderBackbuffer *render_output_backbuffer(void);

/* Convenience headless context builder for tests. */
static RenderCamera render_camera_default(void)
{
    RenderCamera cam = {0, 0, 0u, 0u};
    return cam;
}

static RenderContext make_render_context(const struct SnapshotWorld *snapshot, RenderBackbuffer *target)
{
    RenderContext ctx;
    ctx.snapshot = snapshot;
    ctx.target = target;
    ctx.camera = render_camera_default();
    return ctx;
}

#endif /* REND_API_H */
