#pragma once

#include "system/types.h"
#include "render/rend_backend.h"

struct PresentHandle;
struct RenderContext;
struct RenderInitParams;

struct RenderAdapter
{
    bool (*init)(const RenderInitParams *params, PresentHandle *presenter);
    bool (*frame)(RenderContext *ctx, PresentHandle *presenter);
    void (*shutdown)(PresentHandle *presenter);
};

const RenderAdapter *rend_pick_adapter(const RenderBackendId &backend);
