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
        Append,
        Truncate
    };

public:
    FileStream(const FileStream&) = delete;
    FileStream(FileStream&&) = delete;
    FileStream& operator=(const FileStream&) = delete;
    FileStream& operator=(FileStream&&) = delete;
    virtual ~FileStream();

protected:
    FileStream() = default;

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

    SizeType Size() const;

    virtual bool IsOpen() const = 0;
    virtual bool IsEnd() const = 0;
    virtual SizeType Tell() const = 0;
    virtual void Seek(SizeType pos) = 0;
    virtual void Close() = 0;

    // virtual SizeType Read(void* buf, SizeType count);
    // virtual Array<uint8> ReadBytes();
    // virtual String ReadString();

    // virtual SizeType Write(void* buf, SizeType count);
    // virtual void WriteBytes(const Array<uint8> bytes);
    // virtual void WriteString(const String& str);

protected:
    AccessMode accessMode = AccessMode::Read;
    FileMode fileMode = FileMode::Truncate;
    String pathStr;
    SizeType size;
};

class FileInputStream : public FileStream
{
public:
    explicit FileInputStream(const String& path, AccessMode accessMode = AccessMode::Read, FileMode fileMode = FileMode::Truncate);

    bool IsOpen() const override;
    bool IsEnd() const override;
    SizeType Tell() const override;
    void Seek(SizeType pos) override;
    void Close() override;

    SizeType Read(void* buf, SizeType count);
    Array<uint8> ReadBytes();
    String ReadString();

protected:
    std::ifstream stream;
};


} // namespace IO
