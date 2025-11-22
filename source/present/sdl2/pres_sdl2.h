#pragma once

#include "render/rend_api.h"

bool pres_sdl2_init();
bool pres_sdl2_present(RenderContext &ctx);
void pres_sdl2_shutdown();

u64 pres_sdl2_last_checksum();
u32 pres_sdl2_frame_count();
