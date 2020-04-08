aux_source_directory(. DIR_UTILS)
add_library(Utils ${DIR_UTILS})

target_link_libraries(Utils Math)