#include "platform/plat_api.h"
#include "platform/plat_ids.h"

#include <cstddef>
#include <cstring>
#include <new>
#include <time.h>

struct PlatWindow
{
    int width;
    int height;
    bool should_close;
    uint32 *pixels;

    PlatWindow()
        : width(0)
        , height(0)
        , should_close(false)
        , pixels(0)
    {
    }
};

bool plat_init(const plat_services &services)
{
    (void)services;
    return true;
}

void plat_shutdown()
{
}

PlatWindow *plat_window_create(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return 0;
    }

    PlatWindow *window = new (std::nothrow) PlatWindow();
    if (window == 0)
    {
        return 0;
    }

    const size_t pixel_count = static_cast<size_t>(width) * static_cast<size_t>(height);
    window->pixels = new (std::nothrow) uint32[pixel_count];
    if (window->pixels == 0)
    {
        delete window;
        return 0;
    }

    window->width = width;
    window->height = height;
    std::memset(window->pixels, 0, pixel_count * sizeof(uint32));
    return window;
}

void plat_window_destroy(PlatWindow *window)
{
    if (window == 0)
    {
        return;
    }

    delete[] window->pixels;
    delete window;
}

bool plat_window_pump_events(PlatWindow *window)
{
    if (window == 0)
    {
        return false;
    }

    return !window->should_close;
}

PlatSurface plat_window_get_surface(PlatWindow *window)
{
    PlatSurface surface;
    if (window == 0 || window->pixels == 0)
    {
        surface.pixels = 0;
        surface.width = 0;
        surface.height = 0;
        surface.pitch = 0;
        return surface;
    }

    surface.pixels = static_cast<void *>(window->pixels);
    surface.width = window->width;
    surface.height = window->height;
    surface.pitch = window->width * static_cast<int>(sizeof(uint32));
    return surface;
}

void plat_window_present(PlatWindow *window, const PlatSurface &surface)
{
    (void)window;
    (void)surface;
}

void plat_window_request_close(PlatWindow *window)
{
    if (window == 0)
    {
        return;
    }

    window->should_close = true;
}

void plat_sleep_ms(uint32 milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000u;
    ts.tv_nsec = (milliseconds % 1000u) * 1000000L;
    nanosleep(&ts, 0);
}
