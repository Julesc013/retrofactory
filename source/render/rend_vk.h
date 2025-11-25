#pragma once

#include "render/rend_pick.h"

struct PresentHandle;

bool rend_vk_init(const RenderInitParams *params, PresentHandle *presenter);
bool rend_vk_frame(RenderContext *ctx, PresentHandle *presenter);
void rend_vk_shutdown(PresentHandle *presenter);
const RenderAdapter *rend_vk_adapter(void);
