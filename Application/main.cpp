#include "Application/Application.h"

Application app;
Application *gApp = &app;

int main(int argc, char **argv)
{
    gApp->Init();
    Window::Create({CT_TEXT("Hello World"), 1280, 720});

    gApp->Run();

    return 0;
}