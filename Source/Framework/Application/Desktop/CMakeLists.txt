aux_source_directory(. DIR_APPLICATION_DESKTOP)

add_library(ApplicationDesktop ${DIR_APPLICATION_DESKTOP})

target_link_libraries(ApplicationDesktop glfw)