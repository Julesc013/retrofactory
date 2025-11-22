#pragma once

#include "system/types.h"

enum EnginePhase
{
    EnginePhase_None = 0,
    EnginePhase_Loading,
    EnginePhase_Running,
    EnginePhase_Shutdown
};

struct EngineState
{
    EnginePhase phase;
};

void engine_state_init(EngineState &state);
void engine_state_set_phase(EngineState &state, EnginePhase phase);
