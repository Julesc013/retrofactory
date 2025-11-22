#pragma once

#include "render/rend_api.h"

enum RenderBackend
{
    RenderBackend_Software,
    RenderBackend_DX5
};

bool rend_pick_init(RenderBackend backend);
bool rend_pick_frame(RenderBackend backend, RenderContext &ctx);
void rend_pick_shutdown(RenderBackend backend);
