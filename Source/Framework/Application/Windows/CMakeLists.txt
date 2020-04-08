aux_source_directory(. DIR_APPLICATION_WINDOWS)

add_library(ApplicationWindows ${DIR_APPLICATION_WINDOWS})

target_link_libraries(ApplicationWindows Application)
