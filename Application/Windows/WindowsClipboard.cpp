#include "Application/Windows/WindowsClipboard.h"
#include "Application/Application.h"
#include <windows.h>
#include <windowsx.h>

WindowsClipboard clipboard;
Clipboard *gClipboard = &clipboard;

void Clipboard::Init()
{

}

void WindowsClipboard::SetString(const String &value)
{
    if (value.IsEmpty())
    {
        return;
    }

    auto size = (value.Length() + 1) * sizeof(CharType);
    HANDLE object;
    LPVOID buffer;

    object = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!object)
    {
        CT_LOG(Error, CT_TEXT("Allocate clipboard global handle failed"));
        return;
    }

    buffer = GlobalLock(object);
    if (!buffer)
    {
        CT_LOG(Error, CT_TEXT("Lock clipboard global handle failed"));
        GlobalFree(object);
        return;
    }

    std::memcpy(buffer, *value, size);
    GlobalUnlock(object);

    if (!OpenClipboard(static_cast<HWND>(gApp->GetWindow().GetNativeHandle())))
    {
        CT_LOG(Error, CT_TEXT("Open clipboard failed"));
        GlobalFree(object);
        return;
    }

    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, object);
    CloseClipboard();
}

String WindowsClipboard::GetString()
{
    String result;
    HANDLE object;
    LPVOID buffer;

    if (!OpenClipboard(static_cast<HWND>(gApp->GetWindow().GetNativeHandle())))
    {
        CT_LOG(Error, CT_TEXT("Open clipboard failed"));
        return String();
    }

    object = GetClipboardData(CF_UNICODETEXT);
    if (!object)
    {
        CT_LOG(Error, CT_TEXT("Can not get unicode text from clipboard."));
        CloseClipboard();
        return result;
    }

    buffer = GlobalLock(object);
    if (!buffer)
    {
        CT_LOG(Error, CT_TEXT("Lock clipboard global handle failed"));
        CloseClipboard();
        return result;
    }

    result = String(static_cast<const CharType *>(buffer));

    GlobalUnlock(object);
    CloseClipboard();

    return result;
}
