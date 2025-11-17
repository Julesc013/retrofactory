add_library(runtime STATIC
    rt_config_paths.cpp
    rt_log.cpp
    rt_error.cpp
)

target_include_directories(runtime
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(runtime
    PUBLIC
        system
)
