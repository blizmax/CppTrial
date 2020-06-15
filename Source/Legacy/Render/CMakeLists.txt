aux_source_directory(. DIR_RENDER)
add_library(Render ${DIR_RENDER})

target_link_libraries(Render IO)