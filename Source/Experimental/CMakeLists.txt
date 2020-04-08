if(CT_USE_VULKAN)
    add_executable(LearnVK
        LearnVK.cpp
    )
    target_link_libraries(LearnVK Application)
endif()

