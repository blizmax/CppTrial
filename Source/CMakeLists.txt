add_definitions(-DUNICODE -D_UNICODE)

include_directories("${PROJECT_SOURCE_DIR}/Source")
include_directories("${PROJECT_SOURCE_DIR}/Source/Framework")

include_directories("${CT_EXTERNAL_COMMON}/glad/include")
include_directories("${CT_EXTERNAL_COMMON}/imgui")
include_directories("${CT_EXTERNAL_COMMON}/stb")
include_directories("${CT_EXTERNAL_COMMON}/vulkan/include")
include_directories("${CT_EXTERNAL_COMMON}/${CT_FREETYPE_VERSION}/include")

add_subdirectory(Framework)

if(CT_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

if(WIN32)
    add_subdirectory(Demos)
    add_subdirectory(Experimental)
endif()