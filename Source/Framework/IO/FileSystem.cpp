#include "IO/FileSystem.h"
#include "Core/Time.h"
#include <filesystem>

namespace fs = std::filesystem;

static fs::path ToPath(const String &path)
{
    return fs::path(*path);
}

bool IO::FileSystem::Exists(const String &path)
{
    return fs::exists(ToPath(path));
}

bool IO::FileSystem::IsFile(const String &path)
{
    return fs::is_regular_file(ToPath(path));
}

bool IO::FileSystem::IsDirectory(const String &path)
{
    return fs::is_directory(ToPath(path));
}

bool IO::FileSystem::IsEmpty(const String &path)
{
    return fs::is_empty(ToPath(path));
}

bool IO::FileSystem::CreateDirectory(const String &path)
{
    std::error_code err;
    fs::create_directory(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::CreateDirectories(const String &path)
{
    std::error_code err;
    fs::create_directories(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::Remove(const String &path)
{
    std::error_code err;
    fs::remove(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::RemoveAll(const String &path)
{
    std::error_code err;
    fs::remove_all(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::Copy(const String &from, const String &to, bool overwriteExisting)
{
    std::error_code err;
    if (overwriteExisting)
    {
        fs::copy(ToPath(from), ToPath(to), fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    }
    else
    {
        fs::copy(ToPath(from), ToPath(to), fs::copy_options::recursive);
    }

    return !err;
}

bool IO::FileSystem::Rename(const String &oldPath, const String &newPath)
{
    std::error_code err;
    fs::rename(ToPath(oldPath), ToPath(newPath), err);

    return !err;
}

uint64 IO::FileSystem::GetFileSize(const String &path)
{
    std::error_code err;
    uint64 size = static_cast<uint64>(fs::file_size(ToPath(path), err));
    return err ? 0 : size;
}

int64 IO::FileSystem::GetLastModifiedTime(const String &path)
{
    std::error_code err;
    auto time = fs::last_write_time(ToPath(path), err);
    return err ? -1 : std::chrono::duration_cast<Time::Milliseconds>(time.time_since_epoch()).count();
}

String IO::FileSystem::GetCurrentPath()
{
    auto ret = fs::current_path();
    return String(ret.c_str());
}

String IO::FileSystem::GetTempDirectoryPath()
{
    auto ret = fs::temp_directory_path();
    return String(ret.c_str());
}

String IO::FileSystem::GetFullPath(const String &path)
{
    auto ret = fs::canonical(ToPath(path));
    return String(ret.c_str());
}

String IO::FileSystem::GetAbsolutePath(const String &path)
{
    auto ret = fs::absolute(ToPath(path));
    return String(ret.c_str());
}

bool IO::FileSystem::Iterate(const String &path, std::function<void(const String &path)> callback, bool recursive)
{
    if (!IsDirectory(path))
    {
        return false;
    }

    if (recursive)
    {
        for (auto &entry : fs::recursive_directory_iterator(ToPath(path)))
        {
            callback(String(entry.path().c_str()));
        }
    }
    else
    {
        for (auto &entry : fs::directory_iterator(ToPath(path)))
        {
            callback(String(entry.path().c_str()));
        }
    }
    return true;
}

Array<String> IO::FileSystem::List(const String &path, bool recursive)
{
    Array<String> ret;
    Iterate(
        path, [&ret](const String &p) {
            ret.Add(p);
        },
        recursive);
    return ret;
}