#ifndef GAME_STATE_H
#define GAME_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

struct World;
struct EntitiesState;
struct TransGraph;
struct TravelGraph;

typedef struct GameState
{
    struct World* world;
    struct EntitiesState* entities;

    struct TransGraph* trans_power;
    struct TransGraph* trans_fluid;
    struct TransGraph* trans_data;

    struct TravelGraph* travel_rail;
    struct TravelGraph* travel_road;
    struct TravelGraph* travel_water;
    struct TravelGraph* travel_air;
} GameState;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GAME_STATE_H */
