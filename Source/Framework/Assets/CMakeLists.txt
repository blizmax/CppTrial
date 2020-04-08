aux_source_directory(. DIR_ASSETS)
add_library(Assets ${DIR_ASSETS})

target_link_libraries(Assets IO)