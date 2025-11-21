#include "schedule/tick.h"

bool schedule_tick(Scheduler &scheduler, Tick current_tick)
{
    /* Process and drop due events for now. */
    ScheduledEvent ev;
    while (scheduler_pop_due(scheduler, current_tick, ev))
    {
        /* No-op placeholder; hook for future systems. */
        (void)ev;
    }
    return true;
}
