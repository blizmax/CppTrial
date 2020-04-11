if(CT_BUILD_EXPERIMENTAL)
    add_executable(LearnVK
        LearnVK.cpp
    )
    target_link_libraries(LearnVK Utils)
    target_link_libraries(LearnVK Math)
    target_link_libraries(LearnVK IO)

    target_link_libraries(LearnVK glfw)

    find_library(VULKAN_LIBRARY NAMES vulkan-1 vulkan PATHS ${CT_EXTERNAL_DIR}/vulkan/lib)
    target_link_libraries(LearnVK ${VULKAN_LIBRARY})
endif()

