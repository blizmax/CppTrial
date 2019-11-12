#pragma once

#include "IO/.Package.h"
#include "IO/Path.h"

namespace IO
{

class FileSystem
{
public:
    bool Exists(const Path &path);

    bool IsFile(const Path &path);

    bool IsDirectory(const Path &path);
};

} // namespace IO