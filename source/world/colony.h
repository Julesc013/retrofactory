#pragma once

#include "system/types.h"

struct Colony
{
    u32 colony_id;
    u32 surface_index;
};

void colony_init(Colony &colony, u32 colony_id, u32 surface_index);
