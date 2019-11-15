#pragma once

#include "IO/.Package.h"
#include "IO/Path.h"

namespace IO
{

class FileSystem
{
public:
    //static bool Exists(const Path &path);

    //static bool IsFile(const Path &path);

    //static bool IsDirectory(const Path &path);

    static bool Exists(const String &path);
    
    static bool IsFile(const String &path);

    static bool IsDirectory(const String &path);

    static bool CreateDirectory(const String &path);

    static bool DeleteFile(const String &path);

    static bool RemoveDirectory(const String &path, bool recursively = false);
};

} // namespace IO