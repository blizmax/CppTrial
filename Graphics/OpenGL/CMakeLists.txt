find_package(OpenGL REQUIRED)

aux_source_directory(. DIR_GRAPHICS_OPENGL)
add_library(GraphicsOpenGL
    ${DIR_GRAPHICS_OPENGL}
    ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad.c
)

target_link_libraries(GraphicsOpenGL
    ${OPENGL_LIBRARIES}
)