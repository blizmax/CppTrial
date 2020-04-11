if(WIN32)
    add_subdirectory(Windows)
endif(WIN32)

aux_source_directory(. DIR_APPLICATION)

set(DIR_IMGUI
    ${CT_EXTERNAL_DIR}/imgui/imgui.cpp
    ${CT_EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${CT_EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${CT_EXTERNAL_DIR}/imgui/imgui_widgets.cpp
)

add_library(Application 
    ${DIR_APPLICATION}
    ${DIR_IMGUI}
)

target_link_libraries(Application Math)
target_link_libraries(Application Utils)
target_link_libraries(Application IO)
target_link_libraries(Application Reflection)
target_link_libraries(Application Json)
target_link_libraries(Application Render)
target_link_libraries(Application Assets)

target_link_libraries(Application freetype)

if(WIN32)
    target_link_libraries(Application ApplicationWindows)
endif(WIN32)