add_library(render STATIC
    rnd_api.cpp
    rnd_pick.cpp
    rnd_sw.cpp
)

target_include_directories(render
    PUBLIC
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(render
    PUBLIC
        system
)
