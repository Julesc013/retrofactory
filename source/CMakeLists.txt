add_library(system
    system/rng.cpp
)

target_include_directories(system
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(world
    world/world.cpp
)

target_include_directories(world
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(world
    PUBLIC
        system
)

add_library(saveload
    saveload/saveload.cpp
    saveload/hash.cpp
)

target_include_directories(saveload
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(saveload
    PUBLIC
        world
        system
)

add_library(core
    core/core.cpp
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
)
