#include "Render/FontGenerator.h"
#include <ft2build.h>
#include FT_FREETYPE_H

class FontGenerator::Impl
{
public:
    FT_Library library;
    FT_Face face;

public:
    Impl(const FT_Byte *bytes, FT_Long size)
    {
        FT_Error err;

        err = FT_Init_FreeType(&library);
        if (err)
        {
            CT_EXCEPTION(Render, "Init free type failed.");
            return;
        }

        err = FT_New_Memory_Face(library, bytes, size, 0, &face);
        if (err)
        {
            CT_EXCEPTION(Render, "Create face failed.");
            return;
        }
    }

    void SetPixelSizes(uint32 width, uint32 height)
    {
        FT_Error err = FT_Set_Pixel_Sizes(face, width, height);
        if (err)
        {
            CT_EXCEPTION(Render, "Set pixel size failed.");
        }
    }

    void GenerateFont(const FontGenerator::Options &options)
    {
        int32 fontSize = options.size;
        SetPixelSizes(0, fontSize);

        FT_Error err;
        FT_Int32 loadFlags; //TODO set flags
        FT_Render_Mode renderMode = FT_LOAD_TARGET_MODE(loadFlags);

        for (CharType c : options.characters)
        {
            err = FT_Load_Char(face, (FT_ULong)c, loadFlags);
            if (err)
            {
                CT_EXCEPTION(Render, "Load char failed");
            }

            FT_Render_Glyph(face->glyph, renderMode);
            if (err)
            {
                CT_EXCEPTION(Render, "Render glyph failed");
            }

            FT_GlyphSlot slot = face->glyph;
            //TODO Atlas
        }
    }
};

FontGenerator::FontGenerator(const IO::FileHandle &file)
{
    auto bytes = file.ReadBytes();
    impl = Memory::MakeUnique<Impl>(bytes.GetData(), bytes.Count());
}

FontGenerator::~FontGenerator()
{
    FT_Done_FreeType(impl->library);
}