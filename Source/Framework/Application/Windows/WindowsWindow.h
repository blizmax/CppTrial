#pragma once

#include "Application/Window.h"
#include "Application/Windows/WindowsInput.h"
#include <windows.h>
#include <windowsx.h>

class WindowsWindow : public Window
{
public:
    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;

    virtual Input &GetInput() override
    {
        return input;
    }

    virtual int32 GetPositionX() const override;
    virtual int32 GetPositionY() const override;
    virtual int32 GetWidth() const override;
    virtual int32 GetHeight() const override;

    virtual void SetTitle(const String &title) override;

    virtual void Minimize() override;
    virtual void Maximize() override;
    virtual void Restore() override;
    virtual void Show() override;
    virtual void Hide() override;
    virtual void Focus() override;
    virtual void Flash() override;

    virtual bool Minimized() const override;
    virtual bool Maximized() const override;
    virtual bool Visible() const override;
    virtual bool Focused() const override;

    virtual bool ShouldClose() const override
    {
        return shouldClose;
    }

    virtual void *GetNativeHandle() const override
    {
        return hwnd;
    }

    virtual void CreateNativeWindow(const WindowDesc &desc) override;

private:
    void ProcessWindowResize(int32 width, int32 height);
    void ProcessWindowFocus(bool focused);
    void ProcessFilesDropped(Array<String> &&paths);

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    HWND hwnd;
    HDC hdc;
    WindowsInput input;
    bool shouldClose = false;
};