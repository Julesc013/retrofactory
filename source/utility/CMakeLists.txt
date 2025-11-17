add_library(utility STATIC
    utility_stub.cpp
)

target_include_directories(utility
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)
