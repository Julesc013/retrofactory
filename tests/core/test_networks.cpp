#include "core/trans_net.h"
#include "core/travel_net.h"
#include "core/game_state.h"

int main()
{
    TransGraph power;
    TransGraph fluid;
    trans_graph_init(&power, TRANS_FAM_POWER);
    trans_graph_init(&fluid, TRANS_FAM_FLUID);

    TransNodeId p0 = trans_node_add(&power, 1, 2, 0);
    TransNodeId p1 = trans_node_add(&power, 3, 4, 0);
    TransEdgeId pe = trans_edge_add(&power, p0, p1, 100, 1);

    TransNodeId f0 = trans_node_add(&fluid, 5, 6, 0);
    TransNodeId f1 = trans_node_add(&fluid, 7, 8, 0);
    TransEdgeId fe = trans_edge_add(&fluid, f0, f1, 20, 2);

    if (!trans_node_valid(&power, p0) || !trans_edge_valid(&power, pe))
    {
        trans_graph_free(&power);
        trans_graph_free(&fluid);
        return 1;
    }
    if (!trans_node_valid(&fluid, f0) || !trans_edge_valid(&fluid, fe))
    {
        trans_graph_free(&power);
        trans_graph_free(&fluid);
        return 1;
    }

    GameState gs;
    gs.world = 0;
    gs.entities = 0;
    gs.trans_power = &power;
    gs.trans_fluid = &fluid;
    gs.trans_data = 0;
    gs.travel_rail = 0;
    gs.travel_road = 0;
    gs.travel_water = 0;
    gs.travel_air = 0;

    trans_step_power(&gs);
    trans_step_fluid(&gs);

    TravelGraph rail;
    travel_graph_init(&rail, TRAVEL_FAM_RAIL);
    TravelNodeId t0 = travel_node_add(&rail, 0, 0, 0);
    TravelNodeId t1 = travel_node_add(&rail, 4, 0, 0);
    TravelSegId seg = travel_segment_add_line(&rail, t0, t1, 2, 0, 1, 0);
    if (!travel_segment_valid(&rail, seg))
    {
        travel_graph_free(&rail);
        trans_graph_free(&power);
        trans_graph_free(&fluid);
        return 1;
    }

    travel_graph_maintenance(&rail);

    travel_graph_free(&rail);
    trans_graph_free(&power);
    trans_graph_free(&fluid);
    return 0;
}
