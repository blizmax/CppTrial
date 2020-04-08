#include "Application/Application.h"

Application app;
Application *gApp = &app;

int main(int argc, char **argv)
{
    app.PreInit({CT_TEXT("Hello World"), 1280, 720});

    app.Init();

    app.Run();

    app.Exit();

    return 0;
}