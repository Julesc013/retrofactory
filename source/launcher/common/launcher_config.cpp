#include "launcher/common/launcher_config.h"

#include "runtime/rt_paths.h"

bool launcher_config_load(LauncherConfig &config, LauncherEdition edition, const char *config_path)
{
    launcher_model_init(config.model, edition);
    if (!rt_config_load(&config.runtime, config_path))
    {
        return false;
    }
    return rt_paths_resolve(config.runtime);
}
