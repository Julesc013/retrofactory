#include "core/research.h"

static void add_topic(ResearchState &state, u32 cost)
{
    ResearchTopic topic;
    topic.id = static_cast<ResearchId>(state.topics.size + 1u);
    topic.cost = cost;
    topic.unlocked = false;
    array_push(state.topics, topic);
}

void research_init(ResearchState &state, const PrototypeStore *prototypes)
{
    array_init(state.topics);
    array_reserve(state.topics, 8u);
    state.prototypes = prototypes;

    if (prototypes != 0 && prototypes->research.size > 0u)
    {
        u32 i;
        for (i = 0u; i < prototypes->research.size; ++i)
        {
            add_topic(state, prototypes->research.data[i].cost);
        }
    }
    else
    {
        add_topic(state, 50u);
        add_topic(state, 100u);
        add_topic(state, 200u);
    }

    state.active = state.topics.size > 0u ? state.topics.data[0].id : 0u;
    state.progress = 0u;
}

void research_shutdown(ResearchState &state)
{
    array_free(state.topics);
    state.active = 0u;
    state.progress = 0u;
    state.prototypes = 0;
}

bool research_set_active(ResearchState &state, ResearchId id)
{
    if (id == 0u || id > state.topics.size)
    {
        return false;
    }
    state.active = id;
    state.progress = 0u;
    return true;
}

bool research_is_unlocked(const ResearchState &state, ResearchId id)
{
    if (id == 0u || id > state.topics.size)
    {
        return false;
    }
    const ResearchTopic &topic = state.topics.data[id - 1u];
    return topic.unlocked;
}

void research_tick(ResearchState &state)
{
    if (state.active == 0u || state.active > state.topics.size)
    {
        return;
    }
    ResearchTopic &topic = state.topics.data[state.active - 1u];
    if (topic.unlocked)
    {
        /* Advance to next locked topic if any. */
        u32 i;
        for (i = 0u; i < state.topics.size; ++i)
        {
            ResearchTopic &candidate = state.topics.data[i];
            if (!candidate.unlocked)
            {
                state.active = candidate.id;
                state.progress = 0u;
                break;
            }
        }
        return;
    }

    state.progress += 1u;
    if (state.progress >= topic.cost)
    {
        topic.unlocked = true;
        state.progress = 0u;
    }
}
