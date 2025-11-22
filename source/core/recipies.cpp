#include "core/recipies.h"

void recipies_init(RecipeRegistry &registry, const PrototypeStore *prototypes)
{
    array_init(registry.recipes);
    registry.next_id = 1u;
    registry.prototypes = prototypes;

    if (prototypes != 0 && prototypes->recipes.size > 0u)
    {
        u32 i;
        for (i = 0u; i < prototypes->recipes.size; ++i)
        {
            const RecipePrototype &proto = prototypes->recipes.data[i];
            recipies_register(registry, proto.time, proto.input_cost, proto.output_yield);
        }
    }
    else
    {
        /* Seed with a couple of deterministic defaults. */
        recipies_register(registry, 10u, 1u, 1u);
        recipies_register(registry, 20u, 2u, 3u);
    }
}

void recipies_shutdown(RecipeRegistry &registry)
{
    array_free(registry.recipes);
    registry.next_id = 1u;
    registry.prototypes = 0;
}

RecipeId recipies_register(RecipeRegistry &registry, u32 craft_time, u32 input_cost, u32 output_yield)
{
    Recipe recipe;
    recipe.id = registry.next_id++;
    recipe.craft_time = craft_time;
    recipe.input_cost = input_cost;
    recipe.output_yield = output_yield;
    if (!array_push(registry.recipes, recipe))
    {
        return 0u;
    }
    return recipe.id;
}

const Recipe *recipies_get(const RecipeRegistry &registry, RecipeId id)
{
    u32 i;
    for (i = 0u; i < registry.recipes.size; ++i)
    {
        if (registry.recipes.data[i].id == id)
        {
            return &registry.recipes.data[i];
        }
    }
    return 0;
}
