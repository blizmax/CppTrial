if(CT_BUILD_EXPERIMENTAL)
    add_executable(ShaderReflection ShaderReflection.cpp)
    target_link_libraries(ShaderReflection Application)

    add_executable(LearnVK LearnVK.cpp)
    target_link_libraries(LearnVK Application)

    add_executable(ModelLoader ModelLoader.cpp)
    target_link_libraries(ModelLoader Application)

    add_executable(LearnPBR LearnPBR.cpp)
    target_link_libraries(LearnPBR Application)

    if (CT_USE_VULKAN)
        target_link_libraries(ShaderReflection RenderVulkan)
        target_link_libraries(LearnVK RenderVulkan)
        target_link_libraries(ModelLoader RenderVulkan)
        target_link_libraries(LearnPBR RenderVulkan)
    endif()

endif()

