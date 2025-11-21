#include "world/colony.h"

void colony_init(Colony &colony, u32 colony_id, u32 surface_index)
{
    colony.colony_id = colony_id;
    colony.surface_index = surface_index;
}
