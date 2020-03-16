#include "IO/FileStream.h"
#include <filesystem>

IO::FileStream::FileStream(const String &path, AccessMode accessMode, FileMode fileMode) : pathStr(path), accessMode(accessMode), fileMode(fileMode)
{
}

//===============================================================================
IO::FileInputStream::FileInputStream(const String &path) 
    : FileStream(path, AccessMode::Read, FileMode::None)
{
    std::ios::openmode mode = std::ios::binary | std::ios::in;

    fstream.open(std::filesystem::path(*path), mode);

    if (!fstream.fail())
    {
        fstream.seekg(0, std::ios::end);
        size = static_cast<SizeType>(fstream.tellg());
        fstream.seekg(0, std::ios::beg);
    }
}

bool IO::FileInputStream::IsOpen() const
{
    return fstream.is_open();
}

bool IO::FileInputStream::IsEnd() const
{
    return fstream.eof();
}

SizeType IO::FileInputStream::Tell()
{
    return static_cast<SizeType>(fstream.tellg());
}

void IO::FileInputStream::Seek(SizeType pos)
{
    fstream.seekg(static_cast<std::streampos>(pos));
}

void IO::FileInputStream::Close()
{
    fstream.close();
}

SizeType IO::FileInputStream::Read(void* buf, SizeType count)
{
    fstream.read(static_cast<char8*>(buf), static_cast<std::streamsize>(count));
    return static_cast<SizeType>(fstream.gcount());
}

Array<uint8> IO::FileInputStream::ReadBytes()
{
    Seek(0);
    Array<uint8> arr(size);
    arr.AppendUninitialized(size);
    Read(arr.GetData(), size);
    return arr;
}

String IO::FileInputStream::ReadString()
{
    Seek(0);
    Array<char8> arr(size + 1);
    arr.AppendUninitialized(size + 1);
    Read(arr.GetData(), size);
    arr[size] = 0;
    return StringEncode::UTF8::FromChars(arr);
}

//===============================================================================

IO::FileOutputStream::FileOutputStream(const String &path, FileMode fileMode) 
    : FileStream(path, AccessMode::Write, fileMode)
{
    std::ios::openmode mode = std::ios::binary | std::ios::out;
    if(fileMode == FileMode::Append)
    {
        mode |= std::ios::app;
    }
    else
    {
        mode |= std::ios::trunc;
    }

    fstream.open(std::filesystem::path(*path), mode);

    if (!fstream.fail())
    {
        fstream.seekp(0, std::ios::end);
        size = static_cast<SizeType>(fstream.tellp());
        fstream.seekp(0, std::ios::beg);
    }
}

bool IO::FileOutputStream::IsOpen() const
{
    return fstream.is_open();
}

bool IO::FileOutputStream::IsEnd() const
{
    return fstream.eof();
}

SizeType IO::FileOutputStream::Tell()
{
    return static_cast<SizeType>(fstream.tellp());
}

void IO::FileOutputStream::Seek(SizeType pos)
{
    fstream.seekp(static_cast<std::streampos>(pos));
}

void IO::FileOutputStream::Close()
{
    fstream.close();
}

SizeType IO::FileOutputStream::Write(const void* buf, SizeType count)
{
    fstream.write(static_cast<const char8*>(buf), static_cast<std::streamsize>(count));
    return count;
}

void IO::FileOutputStream::WriteBytes(const Array<uint8>& bytes)
{
    Write(bytes.GetData(), bytes.Count());
}

void IO::FileOutputStream::WriteString(const String& str)
{
    auto arr = StringEncode::UTF8::ToBytes(str);
    Write(arr.GetData(), arr.Count());
}
