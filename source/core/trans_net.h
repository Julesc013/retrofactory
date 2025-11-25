#ifndef TRANS_NET_H
#define TRANS_NET_H

#include "system/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TransFamily {
    TRANS_FAM_POWER = 0,
    TRANS_FAM_FLUID = 1,
    TRANS_FAM_DATA  = 2,
    TRANS_FAM_BELT  = 3,
    TRANS_FAM_COUNT
} TransFamily;

typedef struct TransNodeId { int index; } TransNodeId;
typedef struct TransEdgeId { int index; } TransEdgeId;

typedef struct TransNode {
    TransNodeId  id;
    TransFamily  family;
    int          x;
    int          y;
    int          layer;
    int          island_id;
} TransNode;

typedef struct TransEdge {
    TransEdgeId  id;
    TransFamily  family;
    TransNodeId  a;
    TransNodeId  b;

    int          capacity;
    int          resistance;
} TransEdge;

typedef struct TransGraph {
    TransFamily family;

    TransNode*  nodes;
    int         node_count;
    int         node_capacity;

    TransEdge*  edges;
    int         edge_count;
    int         edge_capacity;
} TransGraph;

void trans_graph_init(TransGraph* g, TransFamily family);
void trans_graph_free(TransGraph* g);

TransNodeId trans_node_add(TransGraph* g, int x, int y, int layer);
TransEdgeId trans_edge_add(TransGraph* g, TransNodeId a, TransNodeId b, int capacity, int resistance);

int trans_node_valid(const TransGraph* g, TransNodeId id);
int trans_edge_valid(const TransGraph* g, TransEdgeId id);

const TransNode* trans_node_get(const TransGraph* g, TransNodeId id);
const TransEdge* trans_edge_get(const TransGraph* g, TransEdgeId id);

struct GameState;

void trans_step_power(struct GameState* g);
void trans_step_fluid(struct GameState* g);
void trans_step_data(struct GameState* g);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TRANS_NET_H */
