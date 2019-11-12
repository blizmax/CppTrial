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

    bool IsEmpty() const
    {
        return pathStr.IsEmpty();
    }

    Path GetParent() const;

    void Swap(Path &other)
    {
        std::swap(pathStr, other.pathStr);
    }

    String GetFullPath() const
    {
        return pathStr;
    }

    String GetFileName() const
    {
        auto index = pathStr.LastIndexOf(CT_TEXT('/'));
        if (index != INDEX_NONE)
        {
            return pathStr.Substring(index + 1);
        }
        return pathStr;
    }

    String GetFileNameWithoutExtension() const
    {
        const String fileName = GetFileName();
        auto index = fileName.LastIndexOf(CT_TEXT('.'));
        if (index != INDEX_NONE)
        {
            return fileName.Substring(0, index);
        }
        return fileName;
    }

    String GetExtension() const
    {
        const String fileName = GetFileName();
        auto index = fileName.LastIndexOf(CT_TEXT('.'));
        if (index != INDEX_NONE)
        {
            return fileName.Substring(index);
        }

        return String();
    }

    Path &Append(const Path &path)
    {
        AppendPrivate(path.pathStr);
        return *this;
    }

    const String &ToString() const
    {
        return pathStr;
    }

    uint32 HashCode() const
    {
        return pathStr.HashCode();
    }

    Path operator+(const Path &other) const
    {
        Path temp = *this;
        return temp.Append(other);
    }

    Path &operator+=(const Path &other)
    {
        return Append(other);
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
    void Format()
    {
        pathStr.ReplaceAll(CT_TEXT('\\'), CT_TEXT('/'));

        //TODO Case sensitive?
    }

    void AppendPrivate(const String &appendPath)
    {
        SizeType len = pathStr.Length();
        const wchar *cstr = pathStr.GetPtr();
        if (len > 0)
        {
            if (cstr[len - 1] != CT_TEXT('/'))
            {
                pathStr.Append(CT_TEXT('/'));
            }
        }

        pathStr.Append(appendPath);
        Format();
    }

private:
    String pathStr;
};
} // namespace IO