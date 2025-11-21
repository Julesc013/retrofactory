#pragma once

#include "system/types.h"
#include "world/surface.h"

struct Planet
{
    u32 planet_id;
    Surface primary_surface;
};

void planet_init(Planet &planet, u32 planet_id);
