#pragma once

#include "render/rend_api.h"

bool rend_dx5_init(void);
bool rend_dx5_frame(RenderContext *ctx);
void rend_dx5_shutdown(void);
