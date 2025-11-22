#include "launcher/ui/macosc_ui.h"

#include <cstdio>

bool launcher_macosc_ui_run(const LauncherConfig &config)
{
    std::printf("Classic Mac launcher edition %d\n", (int)config.model.edition);
    return true;
}
