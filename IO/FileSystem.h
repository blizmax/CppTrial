#pragma once

#include "IO/.Package.h"
#include "IO/Path.h"

namespace IO
{

class FileSystem
{
public:
    static bool Exists(const String &path);

    static bool IsFile(const String &path);

    static bool IsDirectory(const String &path);

    static bool IsEmpty(const String &path);

    static bool CreateDirectory(const String &path);

    static bool CreateDirectories(const String &path);

    static bool DeleteFile(const String &path);

    static bool RemoveDirectory(const String &path, bool recursively = false);

    static String GetCurrentPath();

    static String GetAbsolute(const String &path);
};

} // namespace IO