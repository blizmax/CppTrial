#pragma once

#include "IO/.Package.h"

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

    static bool Remove(const String &path);

    static bool RemoveAll(const String &path);

    static bool Copy(const String &from, const String &to, bool overwriteExisting = false);

    static bool Rename(const String &oldPath, const String& newPath);

    static uint64 GetFileSize(const String &path);

    static int64 GetLastModifiedTime(const String& path);

    static String GetCurrentPath();

    static String GetTempDirectoryPath();

    static String GetFullPath(const String &path);

    static String GetAbsolutePath(const String &path);

    static Array<String> List(const String &path, bool recursive = false);

    static bool Iterate(const String &path, std::function<void(const String &path)> callback, bool recursive = true);
};

} // namespace IO