#pragma once

#include "render/rend_api.h"
#include "render/rend_pick.h"

struct PresentHandle;

bool rend_sdl_init(const RenderInitParams *params, PresentHandle *presenter);
bool rend_sdl_frame(RenderContext *ctx, PresentHandle *presenter);
void rend_sdl_shutdown(PresentHandle *presenter);
const RenderAdapter *rend_sdl_adapter(void);
