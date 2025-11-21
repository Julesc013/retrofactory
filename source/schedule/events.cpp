#include "schedule/events.h"

void scheduler_init(Scheduler &scheduler)
{
    scheduler.count = 0u;
}

bool scheduler_push(Scheduler &scheduler, Tick tick, u32 type, u32 payload)
{
    if (scheduler.count >= kMaxScheduledEvents)
    {
        return false;
    }

    u32 index = scheduler.count;
    scheduler.events[index].tick = tick;
    scheduler.events[index].type = type;
    scheduler.events[index].payload = payload;
    scheduler.count += 1u;

    /* Keep events sorted by tick for determinism (insertion sort). */
    while (index > 0u)
    {
        const u32 prev = index - 1u;
        if (scheduler.events[prev].tick <= scheduler.events[index].tick)
        {
            break;
        }
        ScheduledEvent tmp = scheduler.events[prev];
        scheduler.events[prev] = scheduler.events[index];
        scheduler.events[index] = tmp;
        index = prev;
    }

    return true;
}

bool scheduler_pop_due(Scheduler &scheduler, Tick current_tick, ScheduledEvent &out_event)
{
    if (scheduler.count == 0u)
    {
        return false;
    }

    if (scheduler.events[0].tick > current_tick)
    {
        return false;
    }

    out_event = scheduler.events[0];

    /* Shift remaining events. */
    u32 index;
    for (index = 1u; index < scheduler.count; ++index)
    {
        scheduler.events[index - 1u] = scheduler.events[index];
    }
    scheduler.count -= 1u;
    return true;
}
