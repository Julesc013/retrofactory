#ifndef TRAVEL_NET_H
#define TRAVEL_NET_H

#include "system/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TravelFamily {
    TRAVEL_FAM_RAIL  = 0,
    TRAVEL_FAM_ROAD  = 1,
    TRAVEL_FAM_WATER = 2,
    TRAVEL_FAM_AIR   = 3,
    TRAVEL_FAM_COUNT
} TravelFamily;

typedef struct TravelNodeId { int index; } TravelNodeId;
typedef struct TravelSegId  { int index; } TravelSegId;

typedef enum TravelSegGeomType {
    TRAVEL_SEG_LINE = 0,
    TRAVEL_SEG_ARC  = 1
} TravelSegGeomType;

typedef struct TravelSegGeom {
    TravelSegGeomType type;

    int x0;
    int y0;
    int x1;
    int y1;

    int cx;
    int cy;
    int radius;
    int ang_start;
    int ang_end;
} TravelSegGeom;

typedef struct TravelSegment {
    TravelSegId   id;
    TravelFamily  family;
    TravelNodeId  node_a;
    TravelNodeId  node_b;

    int           lane_count;
    int           one_way_flags;
    int           speed_class;
    int           grade_class;

    TravelSegGeom geom;

    int           length_fixed;
} TravelSegment;

typedef struct TravelNode {
    TravelNodeId id;
    int          x;
    int          y;
    int          elevation;
} TravelNode;

typedef struct TravelGraph {
    TravelFamily family;

    TravelNode*  nodes;
    int          node_count;
    int          node_capacity;

    TravelSegment* segments;
    int            segment_count;
    int            segment_capacity;
} TravelGraph;

void travel_graph_init(TravelGraph* g, TravelFamily family);
void travel_graph_free(TravelGraph* g);

TravelNodeId travel_node_add(TravelGraph* g, int x, int y, int elevation);

TravelSegId  travel_segment_add_line(
    TravelGraph* g,
    TravelNodeId a,
    TravelNodeId b,
    int lane_count,
    int one_way_flags,
    int speed_class,
    int grade_class);

TravelSegId  travel_segment_add_arc(
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
    int ang_end);

int travel_node_valid(const TravelGraph* g, TravelNodeId id);
int travel_segment_valid(const TravelGraph* g, TravelSegId id);

const TravelNode*    travel_node_get(const TravelGraph* g, TravelNodeId id);
const TravelSegment* travel_segment_get(const TravelGraph* g, TravelSegId id);

void travel_graph_maintenance(TravelGraph* g);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TRAVEL_NET_H */
