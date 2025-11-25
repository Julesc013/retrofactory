#include "world/planet.h"
#include "world/surface.h"

void planet_init(Planet &planet, u32 planet_id)
{
    planet.planet_id = planet_id;
    surface_init(planet.primary_surface, 0u, 0u);
}
