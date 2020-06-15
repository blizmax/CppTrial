if(CT_BUILD_EXPERIMENTAL)
    add_executable(LearnVK
        LearnVK.cpp
    )

    target_link_libraries(LearnVK Application)

    if (CT_USE_VULKAN)
        target_link_libraries(LearnVK RenderVulkan)
    endif()

endif()

