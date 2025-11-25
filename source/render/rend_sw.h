#pragma once

#include "render/rend_api.h"
#include "render/rend_pick.h"

struct PresentHandle;

bool rend_sw_init(const RenderInitParams *params, PresentHandle *presenter);
bool rend_sw_frame(RenderContext *ctx, PresentHandle *presenter);
void rend_sw_shutdown(PresentHandle *presenter);
const RenderBackbuffer *rend_sw_backbuffer(void);
const RenderAdapter *rend_sw_adapter(void);
