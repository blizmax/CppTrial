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

aux_source_directory(. DIR_RENDER_VULKAN)
add_library(RenderVulkan
    ${DIR_RENDER_VULKAN}
)

target_link_libraries(RenderVulkan
    ${VULKAN_LIBRARY}
)