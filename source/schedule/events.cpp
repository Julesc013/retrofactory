#include "schedule/events.h"

namespace
{
    void insertion_sort(Scheduler &scheduler, u32 index)
    {
        while (index > 0u)
        {
            const u32 prev = index - 1u;
            const ScheduledEvent &a = scheduler.events[prev];
            const ScheduledEvent &b = scheduler.events[index];
            if (a.tick < b.tick || (a.tick == b.tick && a.type <= b.type))
            {
                break;
            }
            ScheduledEvent tmp = scheduler.events[prev];
            scheduler.events[prev] = scheduler.events[index];
            scheduler.events[index] = tmp;
            index = prev;
        }
    }

    bool schedule_event(Scheduler &scheduler, const ScheduledEvent &ev)
    {
        if (scheduler.count >= kMaxScheduledEvents)
        {
            return false;
        }
        scheduler.events[scheduler.count] = ev;
        insertion_sort(scheduler, scheduler.count);
        scheduler.count += 1u;
        return true;
    }
}

void scheduler_init(Scheduler &scheduler)
{
    scheduler.count = 0u;
    scheduler.processed = 0u;
}

bool scheduler_push(Scheduler &scheduler, Tick tick, u32 type, u32 payload)
{
    ScheduledEvent ev;
    ev.tick = tick;
    ev.type = type;
    ev.payload = payload;
    ev.interval = 0u;
    ev.remaining = 0u;
    return schedule_event(scheduler, ev);
}

bool scheduler_push_interval(Scheduler &scheduler, Tick first_tick, Tick interval, u32 repeats, u32 type, u32 payload)
{
    ScheduledEvent ev;
    ev.tick = first_tick;
    ev.type = type;
    ev.payload = payload;
    ev.interval = interval;
    ev.remaining = repeats;
    return schedule_event(scheduler, ev);
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
    scheduler.processed += 1u;

    /* Reschedule repeating event if necessary. */
    if (out_event.interval > 0u && out_event.remaining > 0u)
    {
        ScheduledEvent next = out_event;
        next.tick = out_event.tick + out_event.interval;
        next.remaining = out_event.remaining - 1u;
        schedule_event(scheduler, next);
    }

    return true;
}

bool scheduler_peek(const Scheduler &scheduler, ScheduledEvent &out_event)
{
    if (scheduler.count == 0u)
    {
        return false;
    }
    out_event = scheduler.events[0];
    return true;
}
