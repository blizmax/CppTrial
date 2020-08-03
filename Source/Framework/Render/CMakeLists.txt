file(GLOB DIR_RENDER_IMPORTERS "Importers/*.cpp")
file(GLOB DIR_RENDER_RENDER_PASSES "RenderPasses/*.cpp")
file(GLOB DIR_RENDER_RENDER_GRAPH "RenderGraph/*.cpp")

aux_source_directory(. DIR_RENDER)
add_library(Render
    ${DIR_RENDER}
    ${DIR_RENDER_IMPORTERS}
    ${DIR_RENDER_RENDER_PASSES}
    ${DIR_RENDER_RENDER_GRAPH}
)

target_link_libraries(Render ${ASSIMP_LIBRARIES})