if(CT_BUILD_EXPERIMENTAL)
    add_executable(LearnVK
        LearnVK.cpp
    )
    target_link_libraries(LearnVK Utils)
    target_link_libraries(LearnVK Math)
    target_link_libraries(LearnVK IO)

    target_link_libraries(LearnVK glfw)

    target_link_libraries(LearnVK RenderVulkan)
endif()

