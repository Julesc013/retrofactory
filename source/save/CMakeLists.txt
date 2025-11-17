add_library(save STATIC
    state_hash.cpp
    save.cpp
)

target_include_directories(save
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)
