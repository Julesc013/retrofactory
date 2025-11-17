#include "runtime/rt_config_paths.h"

RuntimePaths compute_runtime_paths()
{
    RuntimePaths paths;
    paths.base_data_dir = "./data";
    paths.user_config_dir = "./config";
    paths.user_mods_dir = "./mods";
    paths.user_saves_dir = "./saves";
    return paths;
}
