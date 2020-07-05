if(CT_BUILD_EXPERIMENTAL)
    add_executable(LearnVK LearnVK.cpp)
    target_link_libraries(LearnVK Application)

    add_executable(ModelLoader ModelLoader.cpp)
    target_link_libraries(ModelLoader Application)

    if (CT_USE_VULKAN)
        target_link_libraries(LearnVK RenderVulkan)
        target_link_libraries(ModelLoader RenderVulkan)
    endif()

endif()

