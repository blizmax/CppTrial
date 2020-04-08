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

# Render API
if (CT_BUILD_VULKAN)
    set(CT_USE_VULKAN ON)
else()
    set(CT_USE_OPENGL ON)
endif()

set(CT_EXTERNAL_COMMON ${PROJECT_SOURCE_DIR}/External/Common)

# Freetype
set(CT_FREETYPE_VERSION freetype-2.10.1)
add_subdirectory("${CT_EXTERNAL_COMMON}/${CT_FREETYPE_VERSION}")

add_subdirectory(Source)