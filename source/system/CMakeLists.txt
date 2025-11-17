add_library(system STATIC
    rng.cpp
)

target_include_directories(system
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)
