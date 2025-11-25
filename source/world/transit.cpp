#include "world/transit.h"

#include <stddef.h>

void transit_init(TransitGraph &graph)
{
    graph.edges = 0;
    graph.edge_count = 0u;
}

void transit_shutdown(TransitGraph &graph)
{
    if (graph.edges != 0)
    {
        delete[] graph.edges;
        graph.edges = 0;
    }
    graph.edge_count = 0u;
}
