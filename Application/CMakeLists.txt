if(WIN32)
    add_subdirectory(Windows)
endif(WIN32)

aux_source_directory(. DIR_APPLICATION)

add_executable(Application ${DIR_APPLICATION})

target_link_libraries(Application Math)
target_link_libraries(Application Utils)
target_link_libraries(Application IO)
target_link_libraries(Application Reflection)
target_link_libraries(Application Json)

if(CT_USE_OPENGL)
    target_link_libraries(Application GraphicsOpenGL)
endif(CT_USE_OPENGL)

if(WIN32)
    target_link_libraries(Application ApplicationWindows)
endif(WIN32)