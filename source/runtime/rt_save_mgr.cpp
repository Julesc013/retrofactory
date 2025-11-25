#include "runtime/rt_save_mgr.h"

#include <cstdio>
#include <cstring>

bool rt_save_build_descriptor(const RuntimeConfig &config, u32 slot, SaveDescriptor &out_desc)
{
    std::memset(out_desc.path, 0, sizeof(out_desc.path));
    out_desc.slot = slot;
    std::snprintf(out_desc.path, sizeof(out_desc.path), "%s/slot_%u.rfs", config.save_path[0] ? config.save_path : "saves", slot);
    return true;
}
