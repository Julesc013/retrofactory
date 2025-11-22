#include "launcher/common/launcher_config.h"
#include "launcher/ui/gdi_ui.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    LauncherConfig config;
    if (!launcher_config_load(config, Edition_9x, 0))
    {
        return 1;
    }
    return launcher_gdi_ui_run(config) ? 0 : 1;
}
