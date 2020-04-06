add_executable(Demo 
    Demo.cpp
)

target_link_libraries(Demo Application)

if (CT_USE_OPENGL)
    target_link_libraries(Demo RenderOpenGL)
endif()