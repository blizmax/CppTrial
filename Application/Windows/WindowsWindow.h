#pragma once

#include "Application/Window.h"
#include <windows.h>
#include <windowsx.h>

class WindowsWindow : public Window
{
public:
    virtual void OnLoad() override;
    virtual void OnUnload() override;
    virtual void OnTick() override;

    virtual uint32 GetWidth() const override
    {
        //TODO
        return 0;
    }

    virtual uint32 GetHeight() const override
    {
        //TODO
        return 0;
    }

    virtual void *GetNativeHandler() const override
    {
        return hwnd;
    }

    virtual void CreateNativeWindow(const WindowConfig &config) override;

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    HWND hwnd;
    HDC hdc;
    // bool dragging = false;
    // int32 previousX = 0;
    // int32 previousY = 0;
};