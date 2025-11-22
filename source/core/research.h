#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "utility/array.h"

struct ResearchTopic
{
    ResearchId id;
    u32 cost;
    bool unlocked;
};

struct ResearchState
{
    Array<ResearchTopic> topics;
    ResearchId active;
    u32 progress;
};

void research_init(ResearchState &state);
void research_shutdown(ResearchState &state);
void research_tick(ResearchState &state);
bool research_set_active(ResearchState &state, ResearchId id);
bool research_is_unlocked(const ResearchState &state, ResearchId id);
