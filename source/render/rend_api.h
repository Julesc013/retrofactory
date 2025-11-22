#pragma once

#include "engine/snapshot.h"
#include "utility/array.h"

struct RenderBackbuffer
{
    u32 width;
    u32 height;
    Array<u32> pixels;
};

struct RenderContext
{
    const SnapshotWorld *snapshot;
    RenderBackbuffer *target;
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
    return ctx;
}
