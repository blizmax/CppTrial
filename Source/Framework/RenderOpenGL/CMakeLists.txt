find_package(OpenGL REQUIRED)

if(WIN32)
    set(DIR_RENDER_OPENGL_PLATFORM 
       Windows/GLWindowsWindow.cpp
    )
endif()

aux_source_directory(. DIR_RENDER_OPENGL)
add_library(RenderOpenGL
    ${DIR_RENDER_OPENGL}
    ${DIR_RENDER_OPENGL_PLATFORM}
    ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad.c
    ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad_wgl.c
)

target_link_libraries(RenderOpenGL
    ${OPENGL_LIBRARIES}
    Application
)