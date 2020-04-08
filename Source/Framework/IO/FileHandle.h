#pragma once

#include "IO/.Package.h"
#include "IO/FileSystem.h"
#include "IO/FileStream.h"

namespace IO
{
class FileHandle
{
public:
    FileHandle(const FileHandle &) = default;
    FileHandle(FileHandle &&) = default;
    ~FileHandle() = default;
    FileHandle &operator=(const FileHandle &) = default;
    FileHandle &operator=(FileHandle &&) = default;

    FileHandle(const String &path) : pathStr(path)
    {
        Format();
    }

    FileHandle(const CharType *path) : pathStr(path)
    {
        Format();
    }

    void Swap(FileHandle &other)
    {
        std::swap(pathStr, other.pathStr);
    }

    uint64 GetSize() const;
    int64 GetLastModifiedTime() const;

    String GetPath() const;
    String GetFullPath() const;
    String GetAbsolutePath() const;
    String GetFileName() const;
    String GetFileNameWithoutExtension() const;
    String GetExtension() const;
    String GetParentPath() const;
    String GetChildPath(const String& name) const;
    String GetSiblingPath(const String& name) const;

    bool Exists() const;
    bool IsDirectory() const;
    bool IsFile() const;
    bool IsEmpty() const;

    bool CreateDirectory() const;
    bool CreateDirectories() const;
    // Remove file or empty directory.
    bool Remove() const;
    // Remove file or directory recursively.
    bool RemoveAll() const;
    bool CopyTo(const FileHandle& dest, bool overwriteExisting = false) const;
    bool RenameTo(const FileHandle& dest) const;

    FileInputStream Read() const;
    Array<uint8> ReadBytes() const;
    String ReadString() const;
    FileOutputStream Write(bool append = false) const;
    void WriteBytes(const Array<uint8>& bytes, bool append = false) const;
    void WriteString(const String &str, bool append = false) const;

    FileHandle GetParent() const;
    FileHandle GetChild(const String& name) const;
    FileHandle GetSibling(const String& name) const;
    Array<FileHandle> List(bool recursive = false) const;

    template<typename Filter>
    Array<FileHandle> List(Filter filter, bool recursive = false) const
    {
        Array<FileHandle> ret;
        FileSystem::Iterate(pathStr, [&](const String& p){
            if(filter(p))
            {
                ret.Add(FileHandle(p));
            }
        }, recursive);
        return ret;
    }


    FileHandle &Append(const FileHandle &path)
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

    FileHandle operator+(const FileHandle &path) const
    {
        FileHandle temp = *this;
        return temp.Append(path);
    }

    FileHandle &operator+=(const FileHandle &other)
    {
        return Append(other);
    }

    bool operator==(const FileHandle &other) const
    {
        return pathStr == other.pathStr;
    }

    bool operator!=(const FileHandle &other) const
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