#pragma once

#include "system/types.h"

struct ScheduledEvent
{
    Tick tick;
    u32 type;
    u32 payload;
    Tick interval;
    u32 remaining; /* how many repeats remain after this fire */
};

enum
{
    kMaxScheduledEvents = 64
};

struct Scheduler
{
    ScheduledEvent events[kMaxScheduledEvents];
    u32 count;
    u32 processed;
};

void scheduler_init(Scheduler &scheduler);
bool scheduler_push(Scheduler &scheduler, Tick tick, u32 type, u32 payload);
bool scheduler_push_interval(Scheduler &scheduler, Tick first_tick, Tick interval, u32 repeats, u32 type, u32 payload);
bool scheduler_pop_due(Scheduler &scheduler, Tick current_tick, ScheduledEvent &out_event);
bool scheduler_peek(const Scheduler &scheduler, ScheduledEvent &out_event);
