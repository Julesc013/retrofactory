#include "core/travel_net.h"

#include <stdlib.h>

/*
    Travel network subsystem:
    - Maintains deterministic integer graphs for vehicle movement (rail/road/water/air).
    - Placement tools add nodes and line/arc segments; vehicles read immutable geometry.
    - No floating point in authoritative data; lengths use simple integer approximations.
*/

static int travel_abs(int v)
{
    return v < 0 ? -v : v;
}

static int ensure_node_capacity(TravelGraph* g, int min_capacity)
{
    int new_capacity;
    TravelNode* new_nodes;
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
    new_nodes = (TravelNode*)realloc(g->nodes, (size_t)new_capacity * sizeof(TravelNode));
    if (new_nodes == 0)
    {
        return 0;
    }
    g->nodes = new_nodes;
    g->node_capacity = new_capacity;
    return 1;
}

static int ensure_segment_capacity(TravelGraph* g, int min_capacity)
{
    int new_capacity;
    TravelSegment* new_segments;
    if (g == 0)
    {
        return 0;
    }
    if (g->segment_capacity >= min_capacity)
    {
        return 1;
    }
    new_capacity = g->segment_capacity == 0 ? 8 : g->segment_capacity;
    while (new_capacity < min_capacity)
    {
        new_capacity = new_capacity * 2;
    }
    new_segments = (TravelSegment*)realloc(g->segments, (size_t)new_capacity * sizeof(TravelSegment));
    if (new_segments == 0)
    {
        return 0;
    }
    g->segments = new_segments;
    g->segment_capacity = new_capacity;
    return 1;
}

static int compute_line_length(const TravelNode* a, const TravelNode* b)
{
    int dx;
    int dy;
    if (a == 0 || b == 0)
    {
        return 0;
    }
    dx = a->x - b->x;
    dy = a->y - b->y;
    /* Manhattan length keeps this deterministic and integer-only. */
    return travel_abs(dx) + travel_abs(dy);
}

static int compute_arc_length(int radius, int ang_start, int ang_end)
{
    int span;
    if (radius < 0)
    {
        radius = 0;
    }
    span = ang_end - ang_start;
    if (span < 0)
    {
        span = -span;
    }
    /* Integer approximation: degrees * radius. */
    return radius * span;
}

void travel_graph_init(TravelGraph* g, TravelFamily family)
{
    if (g == 0)
    {
        return;
    }
    g->family = family;
    g->nodes = 0;
    g->node_count = 0;
    g->node_capacity = 0;
    g->segments = 0;
    g->segment_count = 0;
    g->segment_capacity = 0;
}

void travel_graph_free(TravelGraph* g)
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
    if (g->segments != 0)
    {
        free(g->segments);
        g->segments = 0;
    }
    g->node_count = 0;
    g->node_capacity = 0;
    g->segment_count = 0;
    g->segment_capacity = 0;
}

TravelNodeId travel_node_add(TravelGraph* g, int x, int y, int elevation)
{
    TravelNodeId id;
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
    g->nodes[index].x = x;
    g->nodes[index].y = y;
    g->nodes[index].elevation = elevation;
    id.index = index;
    return id;
}

TravelSegId travel_segment_add_line(
    TravelGraph* g,
    TravelNodeId a,
    TravelNodeId b,
    int lane_count,
    int one_way_flags,
    int speed_class,
    int grade_class)
{
    TravelSegId id;
    TravelSegment* seg;
    TravelNode* node_a;
    TravelNode* node_b;
    int index;
    id.index = -1;
    if (g == 0)
    {
        return id;
    }
    if (!travel_node_valid(g, a) || !travel_node_valid(g, b))
    {
        return id;
    }
    if (!ensure_segment_capacity(g, g->segment_count + 1))
    {
        return id;
    }
    index = g->segment_count;
    seg = &g->segments[index];
    g->segment_count += 1;

    node_a = &g->nodes[a.index];
    node_b = &g->nodes[b.index];

    seg->id.index = index;
    seg->family = g->family;
    seg->node_a = a;
    seg->node_b = b;
    seg->lane_count = lane_count;
    seg->one_way_flags = one_way_flags;
    seg->speed_class = speed_class;
    seg->grade_class = grade_class;
    seg->geom.type = TRAVEL_SEG_LINE;
    seg->geom.x0 = node_a->x;
    seg->geom.y0 = node_a->y;
    seg->geom.x1 = node_b->x;
    seg->geom.y1 = node_b->y;
    seg->geom.cx = 0;
    seg->geom.cy = 0;
    seg->geom.radius = 0;
    seg->geom.ang_start = 0;
    seg->geom.ang_end = 0;
    seg->length_fixed = compute_line_length(node_a, node_b);

    id.index = index;
    return id;
}

TravelSegId travel_segment_add_arc(
    TravelGraph* g,
    TravelNodeId a,
    TravelNodeId b,
    int lane_count,
    int one_way_flags,
    int speed_class,
    int grade_class,
    int cx, int cy,
    int radius,
    int ang_start,
    int ang_end)
{
    TravelSegId id;
    TravelSegment* seg;
    TravelNode* node_a;
    TravelNode* node_b;
    int index;
    id.index = -1;
    if (g == 0)
    {
        return id;
    }
    if (!travel_node_valid(g, a) || !travel_node_valid(g, b))
    {
        return id;
    }
    if (!ensure_segment_capacity(g, g->segment_count + 1))
    {
        return id;
    }
    index = g->segment_count;
    seg = &g->segments[index];
    g->segment_count += 1;

    node_a = &g->nodes[a.index];
    node_b = &g->nodes[b.index];

    seg->id.index = index;
    seg->family = g->family;
    seg->node_a = a;
    seg->node_b = b;
    seg->lane_count = lane_count;
    seg->one_way_flags = one_way_flags;
    seg->speed_class = speed_class;
    seg->grade_class = grade_class;
    seg->geom.type = TRAVEL_SEG_ARC;
    seg->geom.x0 = node_a->x;
    seg->geom.y0 = node_a->y;
    seg->geom.x1 = node_b->x;
    seg->geom.y1 = node_b->y;
    seg->geom.cx = cx;
    seg->geom.cy = cy;
    seg->geom.radius = radius;
    seg->geom.ang_start = ang_start;
    seg->geom.ang_end = ang_end;
    seg->length_fixed = compute_arc_length(radius, ang_start, ang_end);

    id.index = index;
    return id;
}

int travel_node_valid(const TravelGraph* g, TravelNodeId id)
{
    if (g == 0 || g->nodes == 0)
    {
        return 0;
    }
    return (id.index >= 0) && (id.index < g->node_count);
}

int travel_segment_valid(const TravelGraph* g, TravelSegId id)
{
    if (g == 0 || g->segments == 0)
    {
        return 0;
    }
    return (id.index >= 0) && (id.index < g->segment_count);
}

const TravelNode* travel_node_get(const TravelGraph* g, TravelNodeId id)
{
    if (!travel_node_valid(g, id))
    {
        return 0;
    }
    return &g->nodes[id.index];
}

const TravelSegment* travel_segment_get(const TravelGraph* g, TravelSegId id)
{
    if (!travel_segment_valid(g, id))
    {
        return 0;
    }
    return &g->segments[id.index];
}

void travel_graph_maintenance(TravelGraph* g)
{
    /* Placeholder: future path caches, lane capacity refresh, etc. */
    (void)g;
}
