add_library(engine STATIC
    eng_app.cpp
)

target_include_directories(engine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(engine
    PUBLIC
        core
        system
        runtime
        interface
)
