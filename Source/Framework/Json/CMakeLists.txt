aux_source_directory(. DIR_JSON)
add_library(Json ${DIR_JSON})

target_link_libraries(Json Utils IO)