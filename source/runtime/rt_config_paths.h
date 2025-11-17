#ifndef RUNTIME_RT_CONFIG_PATHS_H
#define RUNTIME_RT_CONFIG_PATHS_H

struct RuntimePaths
{
    const char *base_data_dir;
    const char *user_config_dir;
    const char *user_mods_dir;
    const char *user_saves_dir;
};

RuntimePaths compute_runtime_paths();

#endif /* RUNTIME_RT_CONFIG_PATHS_H */
