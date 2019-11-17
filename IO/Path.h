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

    String GetFullPath() const;
    String GetFileName() const;
    String GetFileNameWithoutExtension() const;
    String GetExtension() const;

    //TODO
    Path GetParent() const;

    bool Exists() const;
    bool IsDirectory() const;
    bool IsFile() const;
    bool IsEmpty() const;

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