#pragma once

#include "system/types.h"

struct EntitiesState
{
    u32 entity_count;
};

void entities_init(EntitiesState &state);
void entities_shutdown(EntitiesState &state);
