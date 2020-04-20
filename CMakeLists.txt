cmake_minimum_required(VERSION 3.1)
project(CppTrial)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS /MP")
endif(MSVC)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "64 bits compiler required.")
endif()

option(CT_BUILD_VULKAN "Build RenderVulkan library." OFF)
option(CT_BUILD_TESTS "Build Tests." OFF)
option(CT_BUILD_EXPERIMENTAL "Build Experimental." ON)

# Render API
if(CT_BUILD_VULKAN)
    set(CT_USE_VULKAN ON)
else()
    set(CT_USE_OPENGL ON)
endif()

set(CT_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/External)

include_directories(${CT_EXTERNAL_DIR}/glad/include)
include_directories(${CT_EXTERNAL_DIR}/imgui)
include_directories(${CT_EXTERNAL_DIR}/stb)
include_directories(${CT_EXTERNAL_DIR}/vulkan/include)
include_directories(${CT_EXTERNAL_DIR}/VulkanMemoryAllocator)

# Freetype
set(CT_FREETYPE_VERSION freetype-2.10.1)
add_subdirectory(${CT_EXTERNAL_DIR}/${CT_FREETYPE_VERSION})
include_directories(${CT_EXTERNAL_DIR}/${CT_FREETYPE_VERSION}/include)

# GLFW
set(CT_GLFW_VERSION glfw-3.3.2)
add_subdirectory(${CT_EXTERNAL_DIR}/${CT_GLFW_VERSION})
include_directories(${CT_EXTERNAL_DIR}/${CT_GLFW_VERSION}/include)

# glslang
add_subdirectory(${CT_EXTERNAL_DIR}/glslang)
include_directories(${CT_EXTERNAL_DIR}/glslang)

add_subdirectory(Source)