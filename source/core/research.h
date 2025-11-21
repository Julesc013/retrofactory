#pragma once

#include "system/types.h"

struct ResearchState
{
    u32 current_topic;
    u32 progress;
};

void research_init(ResearchState &state);
void research_shutdown(ResearchState &state);
void research_tick(ResearchState &state);
