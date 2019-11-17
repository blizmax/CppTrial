#include "IO/Path.h"
#include "IO/FileSystem.h"

String IO::Path::GetFullPath() const
{
    return pathStr;
}

String IO::Path::GetFileName() const
{
    auto index = pathStr.LastIndexOf(CT_TEXT('/'));
    if (index != INDEX_NONE)
    {
        return pathStr.Substring(index + 1);
    }
    return pathStr;
}

String IO::Path::GetFileNameWithoutExtension() const
{
    const String fileName = GetFileName();
    auto index = fileName.LastIndexOf(CT_TEXT('.'));
    if (index != INDEX_NONE)
    {
        return fileName.Substring(0, index);
    }
    return fileName;
}

String IO::Path::GetExtension() const
{
    const String fileName = GetFileName();
    auto index = fileName.LastIndexOf(CT_TEXT('.'));
    if (index != INDEX_NONE)
    {
        return fileName.Substring(index);
    }

    return String();
}

bool IO::Path::Exists() const
{
    return IO::FileSystem::Exists(pathStr);
}

bool IO::Path::IsDirectory() const
{
    return IO::FileSystem::IsDirectory(pathStr);
}

bool IO::Path::IsFile() const
{
    return IO::FileSystem::IsFile(pathStr);
}

bool IO::Path::IsEmpty() const
{
    return IO::FileSystem::IsEmpty(pathStr);
}

void IO::Path::Format()
{
    pathStr.ReplaceAll(CT_TEXT('\\'), CT_TEXT('/'));

    //TODO Case sensitive?
}

void IO::Path::AppendPrivate(const String &appendPath)
{
    SizeType len = pathStr.Length();
    const CharType *cstr = pathStr.CStr();
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