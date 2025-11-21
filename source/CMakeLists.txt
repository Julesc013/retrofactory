add_library(system
    system/rng.cpp
)

target_include_directories(system
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(world
    world/world.cpp
    world/chunk.cpp
    world/region.cpp
    world/surface.cpp
    world/colony.cpp
    world/planet.cpp
    world/transit.cpp
    world/worldgen.cpp
)

target_include_directories(world
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(world
    PUBLIC
        system
)

add_library(schedule
    schedule/events.cpp
    schedule/tick.cpp
)

target_include_directories(schedule
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(schedule
    PUBLIC
        system
)

add_library(saveload
    saveload/saveload.cpp
    saveload/hash.cpp
    saveload/migrate.cpp
)

target_include_directories(saveload
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(saveload
    PUBLIC
        world
        system
        schedule
)

add_library(core
    core/core.cpp
    core/entities.cpp
    core/networks.cpp
    core/recipies.cpp
    core/research.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core
    PUBLIC
        world
        system
        saveload
        schedule
)
