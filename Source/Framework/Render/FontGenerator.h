#pragma once

#include "Render/.Package.h"
#include "Math/Color.h"
#include "IO/FileHandle.h"

class FontGenerator
{
public:
    struct Options
    {
        int32 size = 16;
        bool bold = false;
        bool italic = false;
        String characters;
    };

    FontGenerator(const IO::FileHandle &file);
    ~FontGenerator();

    //void GenerateFont(const Options &options);

private:
    class Impl;
    friend class Impl;
    UPtr<Impl> impl;
};