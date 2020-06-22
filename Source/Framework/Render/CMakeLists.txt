if(WIN32)
    find_library(ASSIMP_LIBRARY NAMES assimp-vc142-mt PATHS ${CT_EXTERNAL_DIR}/assimp/lib)
    if (ASSIMP_LIBRARY)
        set(ASSIMP_FOUND ON)
    endif()
endif()

if(NOT ASSIMP_FOUND)
    message(FATAL_ERROR "Could not find assimp library!")
endif()

file(GLOB DIR_RENDER_IMPORTERS "Importers/*.cpp")

aux_source_directory(. DIR_RENDER)
add_library(Render
    ${DIR_RENDER}
    ${DIR_RENDER_IMPORTERS}
)

target_link_libraries(Render ${ASSIMP_LIBRARY})