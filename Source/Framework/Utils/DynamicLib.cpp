#include "Utils/DynamicLib.h"

#if CT_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER)
#define NOMINMAX
#endif
#include <windows.h>
#elif CT_PLATFORM_OSX || CT_PLATFORM_LINUX
#include <dlfcn.h>
#endif

DynamicLib::DynamicLib(const String &name) : name(name)
{
    Load();
}

void DynamicLib::Load()
{
    if (handle)
        return;

    handle = (DYNLIB_HANDLE)DYNLIB_LOAD(name.CStr());

    if (!handle)
    {
        CT_LOG(Error, CT_TEXT("{Load dynamic lib failed, name:{0}, error:{1}}"), name, GetError());
    }

    CT_CHECK(handle);
}

void DynamicLib::Unload()
{
    if (!handle)
        return;

    if (DYNLIB_UNLOAD(handle))
    {
        CT_LOG(Error, CT_TEXT("{Unload dynamic lib failed, name:{0}, error:{1}}"), name, GetError());
    }
    else
    {
        handle = nullptr;
    }

    CT_CHECK(!handle);
}

void *DynamicLib::GetSymbol(const String &symbol) const
{
    if (!handle)
        return nullptr;

    if (symbol.IsEmpty())
        return nullptr;

    return (void *)DYNLIB_GETSYM(handle, StringEncode::UTF8::ToChars(symbol).GetData());
}

String DynamicLib::GetError() const
{
#if CT_PLATFORM_WIN32
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL);
    String result((CharType*)lpMsgBuf);
    LocalFree(lpMsgBuf);
    return result;
#elif CT_PLATFORM_OSX || CT_PLATFORM_LINUX
    return String(dlerror());
#endif

    return String();
}
