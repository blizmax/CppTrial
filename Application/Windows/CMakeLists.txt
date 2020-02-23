if (CT_USE_OPENGL)
    find_package(OpenGL REQUIRED)

    add_library(ApplicationWindows
        WindowsWindow.cpp
        GLWindowsWindow.cpp
        ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad_wgl.c
    )
    target_link_libraries(ApplicationWindows ${OPENGL_LIBRARIES})
else()
    add_library(ApplicationWindows
        WindowsWindow.cpp
    )
endif(CT_USE_OPENGL)
