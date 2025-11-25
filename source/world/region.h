#pragma once

#include "system/types.h"

struct Region
{
    u32 region_index;
    u32 chunk_start;
    u32 chunk_count;
};

void region_init(Region &region, u32 index, u32 chunk_start, u32 chunk_count);
