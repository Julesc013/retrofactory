#include "render/rnd_sw.h"

#include <cstddef>
#include <cstring>
#include <new>

#include "system/system_base.h"

namespace
{
    PlatWindow *g_sw_window = 0;
    PlatSurface g_sw_surface;
    uint32 *g_sw_framebuffer = 0;
    int g_sw_width = 0;
    int g_sw_height = 0;
    uint32 g_sw_frame_counter = 0u;

    void sw_copy_to_surface()
    {
        if (g_sw_framebuffer == 0 || g_sw_surface.pixels == 0)
        {
            return;
        }

        unsigned char *dest = static_cast<unsigned char *>(g_sw_surface.pixels);
        const size_t row_bytes = static_cast<size_t>(g_sw_width) * sizeof(uint32);
        int y;
        for (y = 0; y < g_sw_height; ++y)
        {
            const uint32 *src_row = g_sw_framebuffer + (y * g_sw_width);
            std::memcpy(dest + y * g_sw_surface.pitch, src_row, row_bytes);
        }

        plat_window_present(g_sw_window, g_sw_surface);
    }
}

bool rnd_sw_init(const RenderConfig &config)
{
    g_sw_window = config.window;
    g_sw_width = config.width;
    g_sw_height = config.height;
    g_sw_surface = plat_window_get_surface(g_sw_window);

    if (g_sw_surface.pixels == 0)
    {
        return false;
    }

    const size_t pixel_count = static_cast<size_t>(g_sw_width) * static_cast<size_t>(g_sw_height);
    g_sw_framebuffer = new (std::nothrow) uint32[pixel_count];
    if (g_sw_framebuffer == 0)
    {
        return false;
    }

    std::memset(g_sw_framebuffer, 0, pixel_count * sizeof(uint32));
    return true;
}

void rnd_sw_render()
{
    if (g_sw_framebuffer == 0)
    {
        return;
    }

    ++g_sw_frame_counter;

    int y;
    for (y = 0; y < g_sw_height; ++y)
    {
        int x;
        for (x = 0; x < g_sw_width; ++x)
        {
            const uint32 r = static_cast<uint32>((x + g_sw_frame_counter) & 0xFFu);
            const uint32 g = static_cast<uint32>((y + g_sw_frame_counter) & 0xFFu);
            const uint32 b = static_cast<uint32>((g_sw_frame_counter) & 0xFFu);
            g_sw_framebuffer[y * g_sw_width + x] = 0xFF000000u | (r << 16) | (g << 8) | b;
        }
    }

    sw_copy_to_surface();
}

void rnd_sw_shutdown()
{
    delete[] g_sw_framebuffer;
    g_sw_framebuffer = 0;
    g_sw_window = 0;
    g_sw_width = 0;
    g_sw_height = 0;
    g_sw_frame_counter = 0u;
}
