#include "Application/Desktop/DesktopClipboard.h"
#include <GLFW/glfw3.h>

DesktopClipboard clipboard;
Clipboard *gClipboard = &clipboard;

void Clipboard::Init()
{
}

void DesktopClipboard::SetString(const String &value)
{
    glfwSetClipboardString(NULL, CT_U8_CSTR(value));
}

String DesktopClipboard::GetString()
{
    return String(glfwGetClipboardString(NULL));
}
