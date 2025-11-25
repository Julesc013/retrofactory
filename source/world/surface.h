#pragma once

#include "system/types.h"
#include "world/region.h"

struct Surface
{
    u32 region_start;
    u32 region_count;
};

void surface_init(Surface &surface, u32 region_start, u32 region_count);
