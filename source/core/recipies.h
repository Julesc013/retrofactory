#pragma once

#include "system/types.h"

struct RecipeRegistry
{
    u32 recipe_count;
};

void recipies_init(RecipeRegistry &registry);
void recipies_shutdown(RecipeRegistry &registry);
