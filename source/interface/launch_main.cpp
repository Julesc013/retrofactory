#include "interface/launch_main.h"

#include "platform/plat_api.h"
#include "render/rnd_api.h"
#include "runtime/rt_log.h"
#include "system/system_base.h"
#include "core/core_api.h"

namespace
{
    const int kDefaultWidth = 640;
    const int kDefaultHeight = 480;
    const uint32 kFrameDelayMs = 16u;
    const uint32 kMaxFrames = 600u;
    const uint64 kLaunchSeed = 0xBEEFCAFEULL;
}

int launch_run()
{
    PlatWindow *window = plat_window_create(kDefaultWidth, kDefaultHeight);
    if (window == 0)
    {
        log_error("Failed to create platform window");
        return 1;
    }

    RenderConfig render_config;
    render_config.width = kDefaultWidth;
    render_config.height = kDefaultHeight;
    render_config.backend_id = RENDER_BACKEND_SW;
    render_config.window = window;

    if (!render_init(render_config))
    {
        log_error("render_init failed");
        plat_window_destroy(window);
        return 1;
    }

    CoreConfig core_config;
    core_config.initial_seed = kLaunchSeed;

    CoreState core_state;
    if (!core_init(core_state, core_config))
    {
        log_error("core_init failed in launch loop");
        render_shutdown();
        plat_window_destroy(window);
        return 1;
    }

    uint32 frame_count = 0u;
    bool running = true;
    while (running)
    {
        running = plat_window_pump_events(window);

        if (!running)
        {
            break;
        }

        if (!core_tick(core_state))
        {
            log_error("core_tick failed");
            break;
        }

        render_frame();
        plat_sleep_ms(kFrameDelayMs);

        ++frame_count;
        if (frame_count >= kMaxFrames)
        {
            plat_window_request_close(window);
            running = false;
        }
    }

    core_shutdown(core_state);
    render_shutdown();
    plat_window_destroy(window);
    return 0;
}
