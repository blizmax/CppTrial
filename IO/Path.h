#pragma once

#include "IO/.Package.h"

namespace IO
{
class Path
{
public:
    Path(const Path &) = default;
    Path(Path &&) = default;
    ~Path() = default;
    Path &operator=(const Path &) = default;
    Path &operator=(Path &&) = default;

    Path(const String &path) : pathStr(path)
    {
        Format();
    }

    Path(const CharType *path) : pathStr(path)
    {
        Format();
    }

    void Swap(Path &other)
    {
        std::swap(pathStr, other.pathStr);
    }

    String GetPathName() const;
    String GetAbsolute() const;
    String GetFileName() const;
    String GetFileNameWithoutExtension() const;
    String GetExtension() const;
    String GetParent() const;
    String GetChild(const String& name) const;
    String GetSibling(const String& name) const;

    //TODO
    Path GetParentPath() const;
    Path GetChildPath(const String& name) const;
    Path GetSiblingPath(const String& name) const;

    bool Exists() const;
    bool IsDirectory() const;
    bool IsFile() const;
    bool IsEmpty() const;

    bool CreateDirectory() const;
    bool CreateDirectories() const;

    Path &Append(const Path &path)
    {
        AppendPrivate(path.pathStr);
        return *this;
    }

    String ToString() const
    {
        return pathStr;
    }

    uint32 HashCode() const
    {
        return pathStr.HashCode();
    }

    Path operator+(const Path &path) const
    {
        Path temp = *this;
        return temp.Append(path);
    }

    Path &operator+=(const Path &path)
    {
        return Append(path);
    }

    bool operator==(const Path &other) const
    {
        return pathStr == other.pathStr;
    }

    bool operator!=(const Path &other) const
    {
        return pathStr != other.pathStr;
    }

private:
    void Format();

    void AppendPrivate(const String &appendPath);

private:
    String pathStr;
};
} // namespace IO