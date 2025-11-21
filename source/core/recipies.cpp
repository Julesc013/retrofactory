#include "core/recipies.h"

void recipies_init(RecipeRegistry &registry)
{
    registry.recipe_count = 0u;
}

void recipies_shutdown(RecipeRegistry &registry)
{
    registry.recipe_count = 0u;
}
