#pragma once

#include "schedule/events.h"

typedef void (*ScheduleHandler)(const ScheduledEvent &event, void *user_data);

bool schedule_tick(Scheduler &scheduler, Tick current_tick, ScheduleHandler handler, void *user_data);
