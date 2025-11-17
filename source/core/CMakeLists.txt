add_library(core STATIC
    core_api.cpp
)

target_include_directories(core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(core
    PUBLIC
        system
)
