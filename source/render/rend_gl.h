#pragma once

#include "render/rend_pick.h"

struct PresentHandle;

bool rend_gl_init(const RenderInitParams *params, PresentHandle *presenter);
bool rend_gl_frame(RenderContext *ctx, PresentHandle *presenter);
void rend_gl_shutdown(PresentHandle *presenter);
const RenderAdapter *rend_gl_adapter(void);
