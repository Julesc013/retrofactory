#pragma once

#include "render/rend_api.h"

bool rend_gl1_init(void);
bool rend_gl1_frame(RenderContext *ctx);
void rend_gl1_shutdown(void);
