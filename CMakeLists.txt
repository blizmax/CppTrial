cmake_minimum_required (VERSION 3.1)
project (CppTrial)

set (CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

include_directories("${PROJECT_SOURCE_DIR}/")

add_subdirectory(Math)
add_subdirectory(Utils)
add_subdirectory(IO)
add_subdirectory(Reflection)
add_subdirectory(Json)

add_subdirectory(Test)