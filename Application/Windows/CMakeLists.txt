set(DIR_APPLICATION_WINDOWS_COMMON 
    WindowsWindow.cpp
    WindowsInput.cpp
    WindowsClipboard.cpp
)

if (CT_USE_OPENGL)
    find_package(OpenGL REQUIRED)

    add_library(ApplicationWindows
        ${DIR_APPLICATION_WINDOWS_COMMON}
        GLWindowsWindow.cpp
        ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad_wgl.c
    )
    target_link_libraries(ApplicationWindows ${OPENGL_LIBRARIES})
else()
    add_library(ApplicationWindows
        ${DIR_APPLICATION_WINDOWS_COMMON}
    )
endif(CT_USE_OPENGL)
