#pragma once

#include "Core/Time.h"
#include "Core/HashMap.h"
#include "IO/FileSystem.h"

namespace IO
{

class FileWatcher
{
public:
    enum class FileStatus
    {
        Created,
        Changed,
        Deleted,
    };

    using FileStatusHandler = std::function<void(const String &, FileStatus)>;

public:
    FileWatcher(const String &watchPath, FileStatusHandler handler, int32 interval = 1000, bool recursive = true)
        : watchPath(watchPath), handler(handler), interval(interval), recursive(recursive)
    {
        if (interval <= 0)
        {
            CT_LOG(Warning, "Watch interval must > 0.");
            this->interval = 1000;
        }
    }

    ~FileWatcher()
    {
        Stop();
    }

    FileWatcher(FileWatcher &&) = delete;
    FileWatcher(const FileWatcher &) = delete;
    FileWatcher &operator=(FileWatcher &&) = delete;
    FileWatcher &operator=(const FileWatcher &) = delete;

    void Start();
    void Stop();

private:
    HashMap<String, int64> pathMap;
    FileStatusHandler handler;
    int32 interval;
    String watchPath;
    bool recursive = true;
    bool running = false;
};

}