#include "world/surface.h"

void surface_init(Surface &surface, u32 region_start, u32 region_count)
{
    surface.region_start = region_start;
    surface.region_count = region_count;
}
