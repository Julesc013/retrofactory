#include "core/entities.h"

void entities_init(EntitiesState &state)
{
    state.entity_count = 0u;
}

void entities_shutdown(EntitiesState &state)
{
    state.entity_count = 0u;
}
