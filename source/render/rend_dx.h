#pragma once

#include "render/rend_pick.h"

struct PresentHandle;

bool rend_dx_init(const RenderInitParams *params, PresentHandle *presenter);
bool rend_dx_frame(RenderContext *ctx, PresentHandle *presenter);
void rend_dx_shutdown(PresentHandle *presenter);
const RenderAdapter *rend_dx_adapter(void);
