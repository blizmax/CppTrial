if (WIN32)
    find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ${CT_EXTERNAL_DIR}/vulkan/lib)
    if (VULKAN_LIBRARY)
        set(VULKAN_FOUND ON)
        message("<${PROJECT_NAME}> Using Vulkan version from External directory.")
    endif()
endif()

if(NOT VULKAN_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
endif()

# if(WIN32)
#     set(DIR_RENDER_OPENGL_PLATFORM 
#        Windows/GLWindowsWindow.cpp
#     )
# endif()

# aux_source_directory(. DIR_RENDER_OPENGL)
# add_library(RenderOpenGL
#     ${DIR_RENDER_OPENGL}
#     ${DIR_RENDER_OPENGL_PLATFORM}
#     ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad.c
#     ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad_wgl.c
# )

# target_link_libraries(RenderOpenGL
#     ${OPENGL_LIBRARIES}
#     Application
# )