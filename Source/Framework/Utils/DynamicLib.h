#pragma once

#include "Utils/.Package.h"

#if CT_PLATFORM_WIN32
struct HINSTANCE__;
typedef struct HINSTANCE__ *hInstance;
#endif

#if CT_PLATFORM_WIN32
#define DYNLIB_HANDLE hInstance
#define DYNLIB_LOAD(a) LoadLibraryEx(a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define DYNLIB_GETSYM(a, b) GetProcAddress(a, b)
#define DYNLIB_UNLOAD(a) !FreeLibrary(a)
#elif CT_PLATFORM_OSX || CT_PLATFORM_LINUX
#define DYNLIB_HANDLE void *
#define DYNLIB_LOAD(a) dlopen(a, RTLD_LAZY | RTLD_GLOBAL)
#define DYNLIB_GETSYM(a, b) dlsym(a, b)
#define DYNLIB_UNLOAD(a) dlclose(a)
#endif

class DynamicLib
{
public:
#if CT_PLATFORM_WIN32
    static constexpr const char8 *EXTENSION = "dll";
#ifdef __MINGW32__
    static constexpr const char8 *PREFIX = "lib";
#else
    static constexpr const char8 *PREFIX = nullptr;
#endif
#elif CT_PLATFORM_OSX
    static constexpr const char8 *EXTENSION = "dylib";
    static constexpr const char8 *PREFIX = "lib";
#elif CT_PLATFORM_LINUX
    static constexpr const char8 *EXTENSION = "so";
    static constexpr const char8 *PREFIX = "lib";
#else
#error Unhandled platform
#endif

    explicit DynamicLib(const String &name);
    ~DynamicLib() = default;

    void Load();
    void Unload();
    void *GetSymbol(const String &symbol) const;

    const String &GetName() const
    {
        return name;
    }

private:
    String GetError() const;

private:
    String name;
    DYNLIB_HANDLE handle = nullptr;
};
