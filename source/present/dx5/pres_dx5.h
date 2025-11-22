#pragma once

#include "render/rend_api.h"

bool pres_dx5_init();
bool pres_dx5_present(RenderContext &ctx);
void pres_dx5_shutdown();

u64 pres_dx5_last_checksum();
u32 pres_dx5_frame_count();
