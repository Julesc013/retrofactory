#include "launcher/common/launcher_config.h"
#include "launcher/common/launcher_detect.h"
#include "launcher/ui/text_ui.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    LauncherConfig config;
    if (!launcher_config_load(config, Edition_1x, 0))
    {
        return 1;
    }
    return launcher_text_ui_run(config) ? 0 : 1;
}
