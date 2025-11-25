#include "launcher/common/launcher_config.h"
#include "launcher/ui/sdl2_ui.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    LauncherConfig config;
    if (!launcher_config_load(config, Edition_Mx, 0))
    {
        return 1;
    }
    return launcher_sdl2_ui_run(config) ? 0 : 1;
}
