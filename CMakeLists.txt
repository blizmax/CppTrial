cmake_minimum_required (VERSION 3.1)
project (CppTrial)

set (CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

if(MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS /MP")
endif(MSVC)

# Render API
set (CT_USE_OPENGL 1)

set (CT_EXTERNAL_COMMON ${PROJECT_SOURCE_DIR}/External/Common)

add_definitions(-DUNICODE -D_UNICODE)

include_directories("${PROJECT_SOURCE_DIR}/")
include_directories("${CT_EXTERNAL_COMMON}/glad/include")
include_directories("${CT_EXTERNAL_COMMON}/imgui")
include_directories("${CT_EXTERNAL_COMMON}/stb")

add_subdirectory(Math)
add_subdirectory(Utils)
add_subdirectory(IO)
add_subdirectory(Reflection)
add_subdirectory(Json)
add_subdirectory(Render)

if(WIN32)
    add_subdirectory(Application)
    add_subdirectory(Demo)
endif(WIN32)

add_subdirectory(Test)