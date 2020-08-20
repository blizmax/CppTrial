# if(WIN32)
#     add_subdirectory(Windows)
# endif(WIN32)
add_subdirectory(Desktop)

aux_source_directory(. DIR_APPLICATION)

set(DIR_IMGUI
    ${CT_EXTERNAL_DIR}/imgui/imgui.cpp
    ${CT_EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${CT_EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${CT_EXTERNAL_DIR}/imgui/imgui_widgets.cpp
)

set(DIR_IMGUI_NODE_EDITOR
    ${CT_EXTERNAL_DIR}/imgui-node-editor/crude_json.cpp
    ${CT_EXTERNAL_DIR}/imgui-node-editor/imgui_canvas.cpp
    ${CT_EXTERNAL_DIR}/imgui-node-editor/imgui_node_editor.cpp
    ${CT_EXTERNAL_DIR}/imgui-node-editor/imgui_node_editor_api.cpp
)

add_library(Application 
    ${DIR_APPLICATION}
    ${DIR_IMGUI}
    ${DIR_IMGUI_NODE_EDITOR}
)

target_link_libraries(Application Math)
target_link_libraries(Application Utils)
target_link_libraries(Application IO)
target_link_libraries(Application Reflection)
target_link_libraries(Application Json)
target_link_libraries(Application Render)
target_link_libraries(Application Assets)

target_link_libraries(Application freetype)

# if(WIN32)
#     target_link_libraries(Application ApplicationWindows)
# endif(WIN32)
target_link_libraries(Application ApplicationDesktop)