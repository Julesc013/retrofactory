#pragma once

#include "system/types.h"

struct ScheduledEvent
{
    Tick tick;
    u32 type;
    u32 payload;
};

enum
{
    kMaxScheduledEvents = 64
};

struct Scheduler
{
    ScheduledEvent events[kMaxScheduledEvents];
    u32 count;
};

void scheduler_init(Scheduler &scheduler);
bool scheduler_push(Scheduler &scheduler, Tick tick, u32 type, u32 payload);
bool scheduler_pop_due(Scheduler &scheduler, Tick current_tick, ScheduledEvent &out_event);
