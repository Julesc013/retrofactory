add_library(system
    system/rng.cpp
)

target_include_directories(system
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(utility
    utility/geom.cpp
    utility/hash.cpp
    utility/path.cpp
    utility/string.cpp
)

target_include_directories(utility
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(utility
    PUBLIC
        system
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
    core/travel_net.c
    core/trans_net.c
)

target_include_directories(world
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(world
    PUBLIC
        utility
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
        utility
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
        utility
        system
)

add_library(mods
    mods/mod_api.cpp
    mods/manifest.cpp
    mods/registry.cpp
    mods/depgraph.cpp
    mods/loader.cpp
    mods/prototypes.cpp
    mods/script/scr_vm.cpp
    mods/script/scr_bind.cpp
)

target_include_directories(mods
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mods
    PUBLIC
        utility
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
        utility
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
        utility
        system
        saveload
        schedule
)

add_library(render
    render/rend_api.cpp
    render/rend_pick.cpp
    render/rend_sw.cpp
    render/rend_sdl.cpp
    render/rend_dx.cpp
    render/rend_gl.cpp
    render/rend_vk.cpp
    render/rend_metal.cpp
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
        utility
        world
)

add_library(present
    present/present_api.cpp
    present/sdl1/pres_sdl1.cpp
    present/sdl2/pres_sdl2.cpp
    present/dx5/pres_dx5.cpp
    present/software/sw_core.c
    present/software/sw_cga.c
    present/software/sw_ega.c
    present/software/sw_vga.c
    present/software/sw_vesa.c
    present/gdi/pres_gdi.c
    present/qdraw/pres_qdraw.c
    present/web/pres_web.cpp
)

target_include_directories(present
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(present
    PUBLIC
        render
        system
        utility
)

add_library(engine
    engine/config.cpp
    engine/engine.cpp
    engine/replay.cpp
    engine/simulate.cpp
    engine/snapshot.cpp
    engine/state.cpp
    engine/retro_runtime.cpp
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(engine
    PUBLIC
        core
        runtime
        utility
        mods
        render
)

# Launcher library to share common logic; entry targets should link this.
add_library(launcher
    launcher/common/launcher_model.cpp
    launcher/common/launcher_config.cpp
    launcher/common/launcher_detect.cpp
    launcher/ui/text_ui.c
    launcher/ui/sdl2_ui.cpp
    launcher/ui/gdi_ui.c
    launcher/ui/macosc_ui.cpp
)

target_include_directories(launcher
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(launcher
    PUBLIC
        runtime
        utility
        engine
)
