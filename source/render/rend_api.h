#pragma once

#include "engine/snapshot.h"

struct RenderContext
{
    const SnapshotWorld *snapshot;
};

bool render_frame(RenderContext &ctx);

/* Convenience headless context builder for tests. */
inline RenderContext make_render_context(const SnapshotWorld *snapshot)
{
    RenderContext ctx;
    ctx.snapshot = snapshot;
    return ctx;
}
