#pragma once

#include "render/rend_pick.h"

struct PresentHandle;

bool rend_metal_init(const RenderInitParams *params, PresentHandle *presenter);
bool rend_metal_frame(RenderContext *ctx, PresentHandle *presenter);
void rend_metal_shutdown(PresentHandle *presenter);
const RenderAdapter *rend_metal_adapter(void);
