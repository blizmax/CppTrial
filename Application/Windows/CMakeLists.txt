aux_source_directory(. DIR_APPLICATION_WINDOWS)

if (CT_USE_OPENGL)
    find_package(OpenGL REQUIRED)

    add_library(ApplicationWindows
        ${DIR_APPLICATION_WINDOWS}
        ${PROJECT_SOURCE_DIR}/External/Common/glad/src/glad_wgl.c
    )

    # target_include_directories(ApplicationWindows PRIVATE
	#     "${CT_EXTERNAL_ROOT}/glad/include"
    # )

    target_link_libraries(ApplicationWindows ${OPENGL_LIBRARIES})
else()
    add_library(ApplicationWindows ${DIR_APPLICATION_WINDOWS})
endif(CT_USE_OPENGL)
