#pragma once

#include "engine/snapshot.h"
#include "utility/array.h"

struct RenderBackbuffer
{
    u32 width;
    u32 height;
    Array<u32> pixels;
};

struct RenderCamera
{
    i32 origin_x;
    i32 origin_y;
    u32 width;
    u32 height;

    RenderCamera() : origin_x(0), origin_y(0), width(0u), height(0u) {}
};

struct RenderContext
{
    const SnapshotWorld *snapshot;
    RenderBackbuffer *target;
    RenderCamera camera;
};

bool render_backbuffer_init(RenderBackbuffer &buffer, u32 width, u32 height);
void render_backbuffer_free(RenderBackbuffer &buffer);
u64 render_backbuffer_checksum(const RenderBackbuffer &buffer);

bool render_frame(RenderContext &ctx);

/* Convenience headless context builder for tests. */
inline RenderContext make_render_context(const SnapshotWorld *snapshot, RenderBackbuffer *target)
{
    RenderContext ctx;
    ctx.snapshot = snapshot;
    ctx.target = target;
    ctx.camera = RenderCamera();
    return ctx;
}
