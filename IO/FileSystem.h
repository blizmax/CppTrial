#pragma once

#include "IO/.Package.h"
#include "IO/Path.h"

namespace IO
{

class FileSystem
{
public:
    static bool Exists(const Path &path);

    static bool IsFile(const Path &path);

    static bool IsDirectory(const Path &path);
};

} // namespace IO