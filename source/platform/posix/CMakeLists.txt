add_library(platform_posix STATIC
    posix_platform.cpp
)

target_include_directories(platform_posix
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(platform_posix
    PUBLIC
        runtime
        system
)

add_executable(retrofactory_posix_stub
    posix_main.cpp
)

target_include_directories(retrofactory_posix_stub
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(retrofactory_posix_stub
    PRIVATE
        platform_posix
        engine
        interface
        render
        runtime
        core
        world
        save
        system
        utility
)
