#include "Application/Desktop/DesktopClipboard.h"
#include <GLFW/glfw3.h>

DesktopClipboard clipboard;
Clipboard *gClipboard = &clipboard;

void Clipboard::Init()
{
}

void DesktopClipboard::SetString(const String &value)
{
    glfwSetClipboardString(NULL, StringEncode::UTF8::ToChars(value).GetData());
}

String DesktopClipboard::GetString()
{
    return StringEncode::UTF8::FromChars(glfwGetClipboardString(NULL));
}
