#pragma once

#include "system/types.h"

struct TransitEdge
{
    u32 from_colony;
    u32 to_colony;
    u32 capacity;
};

struct TransitGraph
{
    TransitEdge *edges;
    u32 edge_count;
};

void transit_init(TransitGraph &graph);
void transit_shutdown(TransitGraph &graph);
