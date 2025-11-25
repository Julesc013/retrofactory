#pragma once

#include "schedule/events.h"
#include "core/game_state.h"

typedef void (*ScheduleHandler)(const ScheduledEvent &event, void *user_data);

bool schedule_tick(Scheduler &scheduler, Tick current_tick, ScheduleHandler handler, void *user_data);
void tick_step_trans_networks(GameState *g);
void tick_step_travel_networks(GameState *g);
