#include "schedule/tick.h"

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
