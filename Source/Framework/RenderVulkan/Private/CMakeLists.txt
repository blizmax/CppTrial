add_library(VulkanShaderCompiler SHARED VulkanShaderCompiler.cpp)

target_compile_definitions(VulkanShaderCompiler PRIVATE -DCT_PRIVATE_EXPORTS)

target_link_libraries(VulkanShaderCompiler
    glslang 
    SPIRV
)