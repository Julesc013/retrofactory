add_library(interface STATIC
    launch_main.cpp
)

target_include_directories(interface
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(interface
    PUBLIC
        render
        runtime
)
