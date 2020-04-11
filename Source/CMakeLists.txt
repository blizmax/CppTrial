add_definitions(-DUNICODE -D_UNICODE)

include_directories("${PROJECT_SOURCE_DIR}/Source")
include_directories("${PROJECT_SOURCE_DIR}/Source/Framework")

add_subdirectory(Framework)

if(CT_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

if(WIN32)
    add_subdirectory(Demos)
    add_subdirectory(Experimental)
endif()