#ifndef CORE_CORE_API_H
#define CORE_CORE_API_H

#include "system/system_base.h"

struct CoreConfig
{
    uint32 reserved;
};

struct CoreState
{
    uint32 reserved;
};

bool core_init(CoreState &state, const CoreConfig &config);
bool core_tick(CoreState &state);
void core_shutdown(CoreState &state);

#endif /* CORE_CORE_API_H */
