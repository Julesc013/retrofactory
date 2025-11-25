#include "launcher/common/launcher_config.h"
#include "launcher/common/launcher_detect.h"
#include "launcher/ui/sdl2_ui.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    LauncherConfig config;
    const LauncherEdition edition = launcher_detect_default();
    if (!launcher_config_load(config, edition, 0))
    {
        return 1;
    }

    if (!launcher_sdl2_ui_run(config))
    {
        return 1;
    }

    return 0;
}
