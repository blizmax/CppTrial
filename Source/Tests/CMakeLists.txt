aux_source_directory(. DIR_TESTS)

add_executable(Tests ${DIR_TEST})

target_link_libraries(Tests 
    Math
    Utils
    IO
    Reflection
    Json
)