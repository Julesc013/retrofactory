#pragma once

#include "system/types.h"
#include "system/ids.h"
#include "utility/array.h"

struct Recipe
{
    RecipeId id;
    u32 craft_time;
    u32 input_cost;
    u32 output_yield;
};

struct RecipeRegistry
{
    Array<Recipe> recipes;
    RecipeId next_id;
};

void recipies_init(RecipeRegistry &registry);
void recipies_shutdown(RecipeRegistry &registry);
RecipeId recipies_register(RecipeRegistry &registry, u32 craft_time, u32 input_cost, u32 output_yield);
const Recipe *recipies_get(const RecipeRegistry &registry, RecipeId id);
