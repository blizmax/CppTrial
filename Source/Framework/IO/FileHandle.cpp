#include "IO/FileHandle.h"

uint64 IO::FileHandle::GetSize() const
{
    return FileSystem::GetFileSize(pathStr);
}

int64 IO::FileHandle::GetLastModifiedTime() const
{
    return FileSystem::GetLastModifiedTime(pathStr);
}

String IO::FileHandle::GetPath() const
{
    return pathStr;
}

String IO::FileHandle::GetFullPath() const
{
    return FileSystem::GetFullPath(pathStr);
}

String IO::FileHandle::GetAbsolutePath() const
{
    return FileSystem::GetAbsolutePath(pathStr);
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
    return pathStr + (pathStr.EndsWith(CT_TEXT("/")) ? CT_TEXT("../") : CT_TEXT("/../"));
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
    return FileSystem::Exists(pathStr);
}

bool IO::FileHandle::IsDirectory() const
{
    return FileSystem::IsDirectory(pathStr);
}

bool IO::FileHandle::IsFile() const
{
    return FileSystem::IsFile(pathStr);
}

bool IO::FileHandle::IsEmpty() const
{
    return FileSystem::IsEmpty(pathStr);
}

bool IO::FileHandle::CreateDirectory() const
{
    return FileSystem::CreateDirectory(pathStr);
}

bool IO::FileHandle::CreateDirectories() const
{
    return FileSystem::CreateDirectories(pathStr);
}

bool IO::FileHandle::Remove() const
{
    return FileSystem::Remove(pathStr);
}

bool IO::FileHandle::RemoveAll() const
{
    return FileSystem::RemoveAll(pathStr);
}

bool IO::FileHandle::CopyTo(const FileHandle& dest, bool overwriteExisting) const
{
    return FileSystem::Copy(pathStr, dest.pathStr, overwriteExisting);
}

bool IO::FileHandle::RenameTo(const FileHandle& dest) const
{
    return FileSystem::Rename(pathStr, dest.pathStr);
}

IO::FileInputStream IO::FileHandle::Read() const
{
    return FileInputStream(pathStr);
}

Array<uint8> IO::FileHandle::ReadBytes() const
{
    FileInputStream stream = Read();
    return stream.ReadBytes();
}

String IO::FileHandle::ReadString() const
{
    FileInputStream stream = Read();
    return stream.ReadString();
}

IO::FileOutputStream IO::FileHandle::Write(bool append) const
{
    if(append)
    {
        return FileOutputStream(pathStr, IO::FileStream::FileMode::Append);
    }
    else
    {
        return FileOutputStream(pathStr, IO::FileStream::FileMode::Truncate);
    }
}

void IO::FileHandle::WriteBytes(const Array<uint8> &bytes, bool append) const
{
    FileOutputStream stream = Write(append);
    stream.WriteBytes(bytes);
}

void IO::FileHandle::WriteString(const String &str, bool append) const
{
    FileOutputStream stream = Write(append);
    stream.WriteString(str);
}

void IO::FileHandle::Format()
{
    pathStr = pathStr.ReplaceAll(CT_TEXT('\\'), CT_TEXT('/'));

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