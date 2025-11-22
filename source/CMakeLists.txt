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

add_library(runtime
    runtime/rt_config.cpp
    runtime/rt_log.cpp
    runtime/rt_profile.cpp
    runtime/rt_error.cpp
    runtime/rt_paths.cpp
    runtime/rt_save_mgr.cpp
)

target_include_directories(runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(runtime
    PUBLIC
        system
)

add_library(mods
    mods/mod_api.cpp
    mods/manifest.cpp
    mods/registry.cpp
    mods/depgraph.cpp
    mods/loader.cpp
    mods/script/scr_vm.cpp
    mods/script/scr_bind.cpp
)

target_include_directories(mods
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mods
    PUBLIC
        system
        runtime
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
        runtime
        mods
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

add_library(render
    render/rend_api.cpp
    render/rend_pick.cpp
    render/rend_sw.cpp
    render/rend_dx5.cpp
    render/rend_gl1.cpp
    render/rend_gl2.cpp
)

target_include_directories(render
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(render
    PUBLIC
        system
        engine
)

add_library(engine
    engine/config.cpp
    engine/engine.cpp
    engine/replay.cpp
    engine/simulate.cpp
    engine/snapshot.cpp
    engine/state.cpp
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(engine
    PUBLIC
        core
        runtime
        mods
        render
)
