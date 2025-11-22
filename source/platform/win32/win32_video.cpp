#include "render/rend_api.h"

#include <cstdio>

namespace
{
    u64 g_last_frame = 0u;
    u32 g_width = 0u;
    u32 g_height = 0u;
}

bool win32_video_init(u32 width, u32 height)
{
    g_width = width;
    g_height = height;
    g_last_frame = 0u;
    return true;
}

bool win32_video_present(const RenderBackbuffer &buffer)
{
    if (buffer.width == 0u || buffer.height == 0u || buffer.pixels.data == 0)
    {
        return false;
    }
    g_last_frame = render_backbuffer_checksum(buffer);
    g_width = buffer.width;
    g_height = buffer.height;
    return true;
}

u64 win32_video_last_checksum()
{
    return g_last_frame;
}

void win32_video_shutdown()
{
    g_width = 0u;
    g_height = 0u;
    g_last_frame = 0u;
}
