#pragma once

#include "Core/General.h"
#include "Core/String.h"

CT_SCOPE_BEGIN

namespace FileSystem
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

    Path(const wchar *path) : pathStr(path)
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
        auto index = pathStr.LastIndexOf(L'/');
        if (index != INDEX_NONE)
        {
            return pathStr.SubString(index + 1);
        }
        return pathStr;
    }

    String GetFileNameWithoutExtension() const
    {
        const String fileName = GetFileName();
        auto index = fileName.LastIndexOf(L'.');
        if (index != INDEX_NONE)
        {
            return fileName.SubString(0, index);
        }
        return fileName;
    }

    String GetExtension() const
    {
        const String fileName = GetFileName();
        auto index = fileName.LastIndexOf(L'.');
        if (index != INDEX_NONE)
        {
            return fileName.SubString(index);
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
        pathStr.ReplaceAll(L'\\', L'/');

        //TODO Case sensitive?
    }

    void AppendPrivate(const String &appendPath)
    {
        SizeType len = pathStr.Length();
        const wchar *cstr = pathStr.GetPtr();
        if (len > 0)
        {
            if (cstr[len - 1] != L'/')
            {
                pathStr.Append(L'/');
            }
        }

        pathStr.Append(appendPath);
        Format();
    }

private:
    String pathStr;
};


bool Exists(const Path& path);

bool IsFile(const Path& path);

bool IsDirectory(const Path& path);




} // namespace FileSystem

CT_SCOPE_END