#include "launcher/ui/sdl2_ui.h"

#include <cstdio>

bool launcher_sdl2_ui_run(const LauncherConfig &config)
{
    std::printf("SDL2 launcher edition %d\n", (int)config.model.edition);
    return true;
}
