add_executable(asset_packer_tool
    asset_packer/packer.cpp
)

target_include_directories(asset_packer_tool
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(asset_packer_tool
    PRIVATE
        mods
        runtime
        utility
        system
)

add_executable(save_viewer
    save_viewer/viewer.cpp
)

target_include_directories(save_viewer
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(save_viewer
    PRIVATE
        core
        saveload
        world
        utility
        system
)

add_executable(world_inspect
    world_inspect/inspect.cpp
)

target_include_directories(world_inspect
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(world_inspect
    PRIVATE
        world
        utility
        system
)

add_executable(mod_scaffold
    mod_sdk/scaffold.cpp
)

target_include_directories(mod_scaffold
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(mod_scaffold
    PRIVATE
        utility
        system
)

add_executable(mod_validate
    mod_sdk/validate.cpp
)

target_include_directories(mod_validate
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(mod_validate
    PRIVATE
        mods
        runtime
        utility
        system
)
