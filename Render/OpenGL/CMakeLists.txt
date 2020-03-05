find_package(OpenGL REQUIRED)

aux_source_directory(. DIR_RENDER_OPENGL)
add_library(RenderOpenGL
    ${DIR_RENDER_OPENGL}
    ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad.c
)

target_link_libraries(RenderOpenGL
    ${OPENGL_LIBRARIES}
    IO
    Render
)