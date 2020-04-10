#include "IO/FileWatcher.h"
#include "Core/Thread.h"

namespace IO
{
void FileWatcher::Start()
{
    pathMap.Clear();
    running = true;

    FileSystem::Iterate(watchPath, [this](const String &path) {
        pathMap.Put(path, FileSystem::GetLastModifiedTime(path));
    }, recursive);

    while (running)
    {
        Thread::SleepFor(interval);

        Array<String> removedPaths;
        for (const auto &entry : pathMap)
        {
            if (!FileSystem::Exists(entry.Key()))
            {
                removedPaths.Add(entry.Key());

                handler(entry.Key(), FileStatus::Deleted);
            }
        }
        for (const auto &path : removedPaths)
        {
            pathMap.Remove(path);
        }

        FileSystem::Iterate(watchPath, [this](const String &path) {
            auto time = FileSystem::GetLastModifiedTime(path);
            if (!pathMap.Contains(path))
            {
                pathMap.Put(path, time);

                handler(path, FileStatus::Created);
            }
            else
            {
                if (pathMap.Get(path) != time)
                {
                    pathMap.Put(path, time);

                    handler(path, FileStatus::Changed);
                }
            }
        }, recursive);
    }
}

void FileWatcher::Stop()
{
    running = false;
}
}