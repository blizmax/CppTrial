#pragma once

#include "IO/.Package.h"
#include <fstream>

namespace IO
{
class FileStream
{
public:
    enum class AccessMode
    {
        Read,
        Write,
    };

    enum class FileMode
    {
        None,
        Append,
        Truncate
    };

public:
    FileStream(const FileStream &) = delete;
    FileStream(FileStream &&) = delete;
    FileStream &operator=(const FileStream &) = delete;
    FileStream &operator=(FileStream &&) = delete;

    virtual ~FileStream() = default;

protected:
    FileStream() = default;

    explicit FileStream(const String &path, AccessMode accessMode, FileMode fileMode);

public:
    AccessMode GetAccessMode() const
    {
        return accessMode;
    }

    FileMode GetFileMode() const
    {
        return fileMode;
    }

    bool IsReadable() const
    {
        return accessMode == AccessMode::Read;
    }

    bool IsWriteable() const
    {
        return accessMode == AccessMode::Write;
    }

    SizeType Size() const
    {
        return size;
    }

    virtual bool IsOpen() const = 0;
    virtual bool IsEnd() const = 0;
    virtual SizeType Tell() = 0;
    virtual void Seek(SizeType pos) = 0;
    virtual void Close() = 0;

protected:
    AccessMode accessMode = AccessMode::Read;
    FileMode fileMode = FileMode::None;
    String pathStr;
    SizeType size = 0;
};

class FileInputStream : public FileStream
{
public:
    explicit FileInputStream(const String &path);

    bool IsOpen() const override;
    bool IsEnd() const override;
    SizeType Tell() override;
    void Seek(SizeType pos) override;
    void Close() override;

    SizeType Read(void *buf, SizeType count);
    Array<uint8> ReadBytes();
    String ReadString();

    template <typename T>
    FileInputStream &operator>>(T &val)
    {
        fstream >> val;
        return *this;
    }

protected:
    std::ifstream fstream;
};

class FileOutputStream : public FileStream
{
public:
    explicit FileOutputStream(const String &path, FileMode fileMode = FileMode::Truncate);

    bool IsOpen() const override;
    bool IsEnd() const override;
    SizeType Tell() override;
    void Seek(SizeType pos) override;
    void Close() override;

    SizeType Write(const void *buf, SizeType count);
    void WriteBytes(const Array<uint8> &bytes);
    void WriteString(const String &str);

protected:
    std::ofstream fstream;
};


}
