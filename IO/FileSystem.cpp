#include "IO/FileSystem.h"
#include <filesystem>

namespace fs = std::filesystem;

static fs::path ToPath(const String& path)
{
    return fs::path(*path);
}

bool IO::FileSystem::Exists(const String& path)
{
    return fs::exists(ToPath(path));
}

bool IO::FileSystem::IsFile(const String& path)
{
    return fs::is_regular_file(ToPath(path));
}

bool IO::FileSystem::IsDirectory(const String& path)
{
    return fs::is_directory(ToPath(path));
}

bool IO::FileSystem::IsEmpty(const String& path)
{
    return fs::is_empty(ToPath(path));
}

bool IO::FileSystem::CreateDirectory(const String& path)
{
    std::error_code err;
    fs::create_directory(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::CreateDirectories(const String& path)
{
    std::error_code err;
    fs::create_directories(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::DeleteFile(const String& path)
{
    if(!IsFile(path))
    {
        return false;
    }

    std::error_code err;
    fs::remove(ToPath(path), err);
    return !err;
}

bool IO::FileSystem::RemoveDirectory(const String& path , bool recursively)
{
    if(!IsDirectory(path))
    {
        return false;
    }

    std::error_code err;
    if(recursively)
    {
        fs::remove_all(ToPath(path), err);
    }
    else
    {
        fs::remove(ToPath(path), err);
    }

    return !err;
}

String IO::FileSystem::GetCurrentPath()
{
    auto ret = fs::current_path();
    return String(ret.c_str());
}

String IO::FileSystem::GetAbsolute(const String &path)
{
    auto ret = fs::absolute(ToPath(path));
    return String(ret.c_str());
}