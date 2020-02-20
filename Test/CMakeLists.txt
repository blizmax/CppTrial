aux_source_directory(. DIR_TEST)

add_executable(Test ${DIR_TEST})

target_link_libraries(Test Math)
target_link_libraries(Test Utils)
target_link_libraries(Test IO)
target_link_libraries(Test Reflection)
target_link_libraries(Test Json)