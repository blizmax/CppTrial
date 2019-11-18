#include "IO/FileHandle.h"

String IO::FileHandle::GetPath() const
{
    return pathStr;
}

String IO::FileHandle::GetFullPath() const
{
    return IO::FileSystem::GetFullPath(pathStr);
}

String IO::FileHandle::GetAbsolutePath() const
{
    return IO::FileSystem::GetAbsolutePath(pathStr);
}

String IO::FileHandle::GetFileName() const
{
    auto index = pathStr.LastIndexOf(CT_TEXT('/'));
    if (index != INDEX_NONE)
    {
        return pathStr.Substring(index + 1);
    }
    return pathStr;
}

String IO::FileHandle::GetFileNameWithoutExtension() const
{
    const String fileName = GetFileName();
    auto index = fileName.LastIndexOf(CT_TEXT('.'));
    if (index != INDEX_NONE)
    {
        return fileName.Substring(0, index);
    }
    return fileName;
}

String IO::FileHandle::GetExtension() const
{
    const String fileName = GetFileName();
    auto index = fileName.LastIndexOf(CT_TEXT('.'));
    if (index != INDEX_NONE)
    {
        return fileName.Substring(index);
    }

    return String();
}

String IO::FileHandle::GetParentPath() const
{
    return pathStr + CT_TEXT("../");
}

String IO::FileHandle::GetChildPath(const String& name) const
{
    if(!pathStr.IsEmpty() && pathStr[pathStr.Length() - 1] != CT_TEXT('/'))
    {
        return pathStr + CT_TEXT('/') + name;
    }

    return pathStr + name;
}

String IO::FileHandle::GetSiblingPath(const String& name) const
{
    return GetParentPath() + name;
}

IO::FileHandle IO::FileHandle::GetParent() const
{
    return FileHandle(GetParentPath());
}

IO::FileHandle IO::FileHandle::GetChild(const String& name) const
{
    return FileHandle(GetChildPath(name));
}

IO::FileHandle IO::FileHandle::GetSibling(const String& name) const
{
    return FileHandle(GetSiblingPath(name));
}

Array<IO::FileHandle> IO::FileHandle::List(bool recursive) const
{
    Array<FileHandle> ret;
    FileSystem::Iterate(pathStr, [&](const String& p){
        ret.Add(FileHandle(p));
    }, recursive);
    return ret;
}

bool IO::FileHandle::Exists() const
{
    return IO::FileSystem::Exists(pathStr);
}

bool IO::FileHandle::IsDirectory() const
{
    return IO::FileSystem::IsDirectory(pathStr);
}

bool IO::FileHandle::IsFile() const
{
    return IO::FileSystem::IsFile(pathStr);
}

bool IO::FileHandle::IsEmpty() const
{
    return IO::FileSystem::IsEmpty(pathStr);
}

bool IO::FileHandle::CreateDirectory() const
{
    return IO::FileSystem::CreateDirectory(pathStr);
}

bool IO::FileHandle::CreateDirectories() const
{
    return IO::FileSystem::CreateDirectories(pathStr);
}

bool IO::FileHandle::Remove() const
{
    return IO::FileSystem::Remove(pathStr);
}

bool IO::FileHandle::RemoveAll() const
{
    return IO::FileSystem::RemoveAll(pathStr);
}

bool IO::FileHandle::CopyTo(const FileHandle& dest, bool overwriteExisting) const
{
    return IO::FileSystem::Copy(pathStr, dest.pathStr, overwriteExisting);
}

bool IO::FileHandle::RenameTo(const FileHandle& dest) const
{
    return IO::FileSystem::Rename(pathStr, dest.pathStr);
}

void IO::FileHandle::Format()
{
    pathStr.ReplaceAll(CT_TEXT('\\'), CT_TEXT('/'));

    //TODO Case sensitive?
}

void IO::FileHandle::AppendPrivate(const String &appendPath)
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