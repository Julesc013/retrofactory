enable_testing()

add_executable(retrofactory_placeholder_test
    placeholder_test.cpp
)

target_include_directories(retrofactory_placeholder_test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

add_test(NAME placeholder_run COMMAND retrofactory_placeholder_test)

add_executable(test_save_load
    determinism/test_save_load.cpp
)

target_include_directories(test_save_load
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(test_save_load
    PRIVATE
        core
        saveload
        system
        world
        engine
)

add_test(NAME determinism_save_load COMMAND test_save_load)

add_executable(test_world
    core/test_world.cpp
)

target_include_directories(test_world
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(test_world
    PRIVATE
        core
        saveload
        system
        world
)

add_test(NAME core_world COMMAND test_world)

add_executable(test_entities
    core/test_entity.cpp
)

target_include_directories(test_entities PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_entities PRIVATE core world system utility)
add_test(NAME core_entities COMMAND test_entities)

add_executable(test_networks
    core/test_networks.cpp
)
target_include_directories(test_networks PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_networks PRIVATE core world system utility)
add_test(NAME core_networks COMMAND test_networks)

add_executable(test_research
    core/test_research.cpp
)
target_include_directories(test_research PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_research PRIVATE core system utility)
add_test(NAME core_research COMMAND test_research)

add_executable(test_rng
    core/test_rng.cpp
)
target_include_directories(test_rng PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_rng PRIVATE system)
add_test(NAME core_rng COMMAND test_rng)

add_executable(test_render_sw
    render/test_render_sw.cpp
)
target_include_directories(test_render_sw PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_render_sw PRIVATE render core world utility system)
add_test(NAME render_sw COMMAND test_render_sw)

add_executable(test_presenters
    render/test_presenters.cpp
)
target_include_directories(test_presenters PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_presenters PRIVATE present render core world utility system)
add_test(NAME render_presenters COMMAND test_presenters)

add_executable(test_replay_basic
    determinism/test_replay_basic.cpp
)
target_include_directories(test_replay_basic PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_replay_basic PRIVATE engine core saveload world utility system render)
add_test(NAME determinism_replay_basic COMMAND test_replay_basic)

add_executable(test_replay_cross
    determinism/test_replay_cross.cpp
)
target_include_directories(test_replay_cross PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_replay_cross PRIVATE engine core saveload world utility system render)
add_test(NAME determinism_replay_cross COMMAND test_replay_cross)

add_executable(test_replay_editions
    determinism/test_replay_editions.cpp
)
target_include_directories(test_replay_editions PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_replay_editions PRIVATE engine core saveload world utility system render)
add_test(NAME determinism_replay_editions COMMAND test_replay_editions)

add_test(NAME tools_asset_packer COMMAND asset_packer_tool)
add_test(NAME tools_save_viewer_demo COMMAND save_viewer --demo)

add_executable(test_worldgen
    macrosim/test_transit.cpp
)
target_include_directories(test_worldgen PRIVATE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(test_worldgen PRIVATE world utility system)
add_test(NAME worldgen_stability COMMAND test_worldgen)
