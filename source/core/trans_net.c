#include "core/trans_net.h"
#include "core/game_state.h"

#include <stdlib.h>

/*
    Transmission network subsystem:
    - Deterministic integer graphs carrying power/fluids/data (dense, grid-ish).
    - Nodes/edges are managed with simple contiguous arrays for portability.
    - Steppers are placeholders that will host flow/propagation logic later.
*/

static int ensure_node_capacity(TransGraph* g, int min_capacity)
{
    int new_capacity;
    TransNode* new_nodes;
    if (g == 0)
    {
        return 0;
    }
    if (g->node_capacity >= min_capacity)
    {
        return 1;
    }
    new_capacity = g->node_capacity == 0 ? 8 : g->node_capacity;
    while (new_capacity < min_capacity)
    {
        new_capacity = new_capacity * 2;
    }
    new_nodes = (TransNode*)realloc(g->nodes, (size_t)new_capacity * sizeof(TransNode));
    if (new_nodes == 0)
    {
        return 0;
    }
    g->nodes = new_nodes;
    g->node_capacity = new_capacity;
    return 1;
}

static int ensure_edge_capacity(TransGraph* g, int min_capacity)
{
    int new_capacity;
    TransEdge* new_edges;
    if (g == 0)
    {
        return 0;
    }
    if (g->edge_capacity >= min_capacity)
    {
        return 1;
    }
    new_capacity = g->edge_capacity == 0 ? 8 : g->edge_capacity;
    while (new_capacity < min_capacity)
    {
        new_capacity = new_capacity * 2;
    }
    new_edges = (TransEdge*)realloc(g->edges, (size_t)new_capacity * sizeof(TransEdge));
    if (new_edges == 0)
    {
        return 0;
    }
    g->edges = new_edges;
    g->edge_capacity = new_capacity;
    return 1;
}

void trans_graph_init(TransGraph* g, TransFamily family)
{
    if (g == 0)
    {
        return;
    }
    g->family = family;
    g->nodes = 0;
    g->node_count = 0;
    g->node_capacity = 0;
    g->edges = 0;
    g->edge_count = 0;
    g->edge_capacity = 0;
}

void trans_graph_free(TransGraph* g)
{
    if (g == 0)
    {
        return;
    }
    if (g->nodes != 0)
    {
        free(g->nodes);
        g->nodes = 0;
    }
    if (g->edges != 0)
    {
        free(g->edges);
        g->edges = 0;
    }
    g->node_count = 0;
    g->node_capacity = 0;
    g->edge_count = 0;
    g->edge_capacity = 0;
}

TransNodeId trans_node_add(TransGraph* g, int x, int y, int layer)
{
    TransNodeId id;
    int index;
    if (g == 0)
    {
        id.index = -1;
        return id;
    }
    if (!ensure_node_capacity(g, g->node_count + 1))
    {
        id.index = -1;
        return id;
    }
    index = g->node_count;
    g->node_count += 1;
    g->nodes[index].id.index = index;
    g->nodes[index].family = g->family;
    g->nodes[index].x = x;
    g->nodes[index].y = y;
    g->nodes[index].layer = layer;
    g->nodes[index].island_id = -1;
    id.index = index;
    return id;
}

TransEdgeId trans_edge_add(TransGraph* g, TransNodeId a, TransNodeId b, int capacity, int resistance)
{
    TransEdgeId id;
    int index;
    TransEdge* edge;
    id.index = -1;
    if (g == 0)
    {
        return id;
    }
    if (!trans_node_valid(g, a) || !trans_node_valid(g, b))
    {
        return id;
    }
    if (!ensure_edge_capacity(g, g->edge_count + 1))
    {
        return id;
    }
    index = g->edge_count;
    g->edge_count += 1;
    edge = &g->edges[index];
    edge->id.index = index;
    edge->family = g->family;
    edge->a = a;
    edge->b = b;
    edge->capacity = capacity;
    edge->resistance = resistance;
    id.index = index;
    return id;
}

int trans_node_valid(const TransGraph* g, TransNodeId id)
{
    if (g == 0 || g->nodes == 0)
    {
        return 0;
    }
    return (id.index >= 0) && (id.index < g->node_count);
}

int trans_edge_valid(const TransGraph* g, TransEdgeId id)
{
    if (g == 0 || g->edges == 0)
    {
        return 0;
    }
    return (id.index >= 0) && (id.index < g->edge_count);
}

const TransNode* trans_node_get(const TransGraph* g, TransNodeId id)
{
    if (!trans_node_valid(g, id))
    {
        return 0;
    }
    return &g->nodes[id.index];
}

const TransEdge* trans_edge_get(const TransGraph* g, TransEdgeId id)
{
    if (!trans_edge_valid(g, id))
    {
        return 0;
    }
    return &g->edges[id.index];
}

static void trans_graph_placeholder_step(TransGraph* g)
{
    int i;
    if (g == 0)
    {
        return;
    }
    /* Simple deterministic touch to mark connectivity buckets. */
    for (i = 0; i < g->node_count; ++i)
    {
        g->nodes[i].island_id = g->nodes[i].family;
    }
    /* Edges kept intact; future flow/pressure/signal logic will land here. */
}

void trans_step_power(struct GameState* g)
{
    TransGraph* graph = g != 0 ? g->trans_power : 0;
    trans_graph_placeholder_step(graph);
}

void trans_step_fluid(struct GameState* g)
{
    TransGraph* graph = g != 0 ? g->trans_fluid : 0;
    trans_graph_placeholder_step(graph);
}

void trans_step_data(struct GameState* g)
{
    TransGraph* graph = g != 0 ? g->trans_data : 0;
    trans_graph_placeholder_step(graph);
}
