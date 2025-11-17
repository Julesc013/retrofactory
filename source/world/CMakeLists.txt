add_library(world STATIC
    world_space.cpp
    world_grid.cpp
)

target_include_directories(world
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(world
    PUBLIC
        system
)
