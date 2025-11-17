#ifndef RENDER_RND_SW_H
#define RENDER_RND_SW_H

#include "render/rnd_api.h"

bool rnd_sw_init(const RenderConfig &config);
void rnd_sw_render();
void rnd_sw_shutdown();

#endif /* RENDER_RND_SW_H */
