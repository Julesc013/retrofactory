#include "core/research.h"

void research_init(ResearchState &state)
{
    state.current_topic = 0u;
    state.progress = 0u;
}

void research_shutdown(ResearchState &state)
{
    state.current_topic = 0u;
    state.progress = 0u;
}

void research_tick(ResearchState &state)
{
    /* Simple deterministic progression placeholder. */
    state.progress += 1u;
}
