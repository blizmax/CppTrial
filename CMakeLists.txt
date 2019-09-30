cmake_minimum_required (VERSION 3.1)
project (CppTrial)

set (CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 17)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

include_directories("${PROJECT_SOURCE_DIR}/")
#include_directories("${PROJECT_SOURCE_DIR}/Core")

# add_subdirectory(Core)
# target_link_libraries(Core)

add_executable(Main main.cpp)