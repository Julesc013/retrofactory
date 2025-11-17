enable_testing()

add_executable(retrofactory_placeholder_test
    placeholder_test.cpp
)

target_include_directories(retrofactory_placeholder_test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

add_test(NAME placeholder_run COMMAND retrofactory_placeholder_test)
