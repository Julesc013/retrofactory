#include "launcher/ui/gdi_ui.h"

#include <cstdio>

bool launcher_gdi_ui_run(const LauncherConfig &config)
{
    std::printf("GDI launcher edition %d\n", (int)config.model.edition);
    return true;
}
