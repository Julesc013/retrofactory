#include "engine/state.h"

void engine_state_init(EngineState &state)
{
    state.phase = EnginePhase_Loading;
}

void engine_state_set_phase(EngineState &state, EnginePhase phase)
{
    state.phase = phase;
}
