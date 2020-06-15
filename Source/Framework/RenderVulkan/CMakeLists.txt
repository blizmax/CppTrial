if(WIN32)
    find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ${CT_EXTERNAL_DIR}/vulkan/lib)
    if (VULKAN_LIBRARY)
        set(VULKAN_FOUND ON)
        message("<${PROJECT_NAME}> Using Vulkan version from External directory.")
    endif()
endif()

if(NOT VULKAN_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
endif()

add_subdirectory(Private)

aux_source_directory(. DIR_RENDER_VULKAN)
file(GLOB DIR_RENDER_CORE "../RenderCore/*.cpp")

set(DIR_RENDER_VULKAN_PLATFORM Desktop/VulkanDesktopWindow.cpp)

add_library(RenderVulkan
    ${DIR_RENDER_CORE}
    ${DIR_RENDER_VULKAN}
    ${DIR_RENDER_VULKAN_PLATFORM}
)

target_link_libraries(RenderVulkan
    ${VULKAN_LIBRARY}
)