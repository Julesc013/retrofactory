#include "world/region.h"

void region_init(Region &region, u32 index, u32 chunk_start, u32 chunk_count)
{
    region.region_index = index;
    region.chunk_start = chunk_start;
    region.chunk_count = chunk_count;
}
