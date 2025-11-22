#include "core/research.h"

int main()
{
    ResearchState state;
    research_init(state);

    u32 i;
    for (i = 0u; i < 300u; ++i)
    {
        research_tick(state);
    }

    if (!research_is_unlocked(state, 1u))
    {
        research_shutdown(state);
        return 1;
    }

    research_shutdown(state);
    return 0;
}
