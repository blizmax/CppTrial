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

# Render API
set(CT_USE_OPENGL 1)
option(CT_USE_VULKAN "Build RenderVulkan library." OFF)

set(CT_EXTERNAL_COMMON ${PROJECT_SOURCE_DIR}/External/Common)

add_definitions(-DUNICODE -D_UNICODE)

include_directories("${PROJECT_SOURCE_DIR}/")
include_directories("${CT_EXTERNAL_COMMON}/glad/include")
include_directories("${CT_EXTERNAL_COMMON}/imgui")
include_directories("${CT_EXTERNAL_COMMON}/stb")
include_directories("${CT_EXTERNAL_COMMON}/vulkan/include")

# Freetype
set(CT_FREETYPE_VERSION freetype-2.10.1)
include_directories("${CT_EXTERNAL_COMMON}/${CT_FREETYPE_VERSION}/include")
add_subdirectory("${CT_EXTERNAL_COMMON}/${CT_FREETYPE_VERSION}")

add_subdirectory(Math)
add_subdirectory(Utils)
add_subdirectory(IO)
add_subdirectory(Reflection)
add_subdirectory(Json)
add_subdirectory(Render)
add_subdirectory(RenderOpenGL)

if(CT_USE_VULKAN)
    add_subdirectory(RenderVulkan)
endif()

add_subdirectory(Test)

if(WIN32)
    add_subdirectory(Application)
    add_subdirectory(Demo)
endif()

if(WIN32)
    add_subdirectory(Experimental)
endif()