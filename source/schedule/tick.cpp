#include "schedule/tick.h"
#include "core/trans_net.h"
#include "core/travel_net.h"

bool schedule_tick(Scheduler &scheduler, Tick current_tick, ScheduleHandler handler, void *user_data)
{
    ScheduledEvent ev;
    while (scheduler_pop_due(scheduler, current_tick, ev))
    {
        if (handler)
        {
            handler(ev, user_data);
        }
    }
    return true;
}

void tick_step_trans_networks(GameState *g)
{
    if (g == 0)
    {
        return;
    }
    trans_step_power(g);
    trans_step_fluid(g);
    trans_step_data(g);
}

void tick_step_travel_networks(GameState *g)
{
    if (g == 0)
    {
        return;
    }
    if (g->travel_rail != 0)
    {
        travel_graph_maintenance(g->travel_rail);
    }
    if (g->travel_road != 0)
    {
        travel_graph_maintenance(g->travel_road);
    }
    if (g->travel_water != 0)
    {
        travel_graph_maintenance(g->travel_water);
    }
    if (g->travel_air != 0)
    {
        travel_graph_maintenance(g->travel_air);
    }
}
