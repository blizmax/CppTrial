#pragma once

#include "Application/.Package.h"
#include "Application/Window.h"

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    virtual void Init();
    virtual void Run();

    // virtual void CreateWindow();
    // virtual Window& GetWindow();



};

extern Application *gApp;