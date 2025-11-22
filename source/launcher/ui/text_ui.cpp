#include "launcher/ui/text_ui.h"

#include <cstdio>

bool launcher_text_ui_run(const LauncherConfig &config)
{
    std::printf("Launching edition %d using data path %s\n", (int)config.model.edition, config.runtime.data_path);
    return true;
}
