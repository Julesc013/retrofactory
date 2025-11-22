#include "render/rend_api.h"

namespace
{
    u64 g_last_frame = 0u;
}

bool posix_video_present(const RenderBackbuffer &buffer)
{
    if (buffer.width == 0u || buffer.height == 0u || buffer.pixels.data == 0)
    {
        return false;
    }
    g_last_frame = render_backbuffer_checksum(buffer);
    return true;
}

u64 posix_video_last_checksum()
{
    return g_last_frame;
}
