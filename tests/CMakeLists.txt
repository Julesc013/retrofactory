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
