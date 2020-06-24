file(GLOB DIR_RENDER_IMPORTERS "Importers/*.cpp")

aux_source_directory(. DIR_RENDER)
add_library(Render
    ${DIR_RENDER}
    ${DIR_RENDER_IMPORTERS}
)

target_link_libraries(Render ${ASSIMP_LIBRARIES})