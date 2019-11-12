aux_source_directory(. DIR_IO)
add_library(IO ${DIR_IO})

target_link_libraries(IO Utils)