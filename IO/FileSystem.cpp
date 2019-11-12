#include "IO/FileSystem.h"
#include <filesystem>

namespace fs = std::filesystem;

static fs::path StdPath(const IO::Path& path)
{
    return fs::path(*path.ToString());
}

bool IO::FileSystem::Exists(const Path& path)
{
    return fs::exists(StdPath(path));
}