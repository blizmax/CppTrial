add_executable(ShaderToy 
    ShaderToy.cpp
)

target_link_libraries(ShaderToy Application)

if (CT_USE_VULKAN)
    target_link_libraries(ShaderToy RenderVulkan)
endif()