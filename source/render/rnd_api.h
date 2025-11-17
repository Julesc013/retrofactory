#ifndef RENDER_RND_API_H
#define RENDER_RND_API_H

#include "platform/plat_api.h"

enum RenderBackendId
{
    RENDER_BACKEND_SW = 0
};

struct RenderConfig
{
    int width;
    int height;
    int backend_id;
    PlatWindow *window;
};

bool render_init(const RenderConfig &config);
void render_frame();
void render_shutdown();

#endif /* RENDER_RND_API_H */
