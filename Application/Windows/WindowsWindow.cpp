#include "Application/Windows/WindowsWindow.h"
#include "Application/Application.h"
#include <tchar.h>

void WindowsWindow::CreateNativeWindow(const WindowConfig &config)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("WindowsWindow");

    RegisterClassEx(&wc);

    int height_adjust = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER);
    int width_adjust = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

    hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,              // name of the window class
        config.title.CStr(),           // title of the window
        WS_OVERLAPPEDWINDOW,           // window style
        CW_USEDEFAULT,                 // x-position of the window
        CW_USEDEFAULT,                 // y-position of the window
        config.width + width_adjust,   // width of the window
        config.height + height_adjust, // height of the window
        NULL,                          // we have no parent window, NULL
        NULL,                          // we aren't using menus, NULL
        hInstance,                     // application handle
        this);                         // pass pointer to current object

    hdc = GetDC(hwnd);

    // Allow drag
    DragAcceptFiles(hwnd, TRUE);

    ShowWindow(hwnd, SW_SHOW);
}

void WindowsWindow::OnLoad()
{
}

void WindowsWindow::OnUnload()
{
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
}

void WindowsWindow::OnUpdate()
{
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    WindowsWindow *thisWindow;
    if (message == WM_NCCREATE)
    {
        thisWindow = static_cast<WindowsWindow *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(thisWindow)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        thisWindow = reinterpret_cast<WindowsWindow *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    switch (message)
    {
    case WM_CHAR:
    {
        thisWindow->input.ProcessKeyTyped((int32)wParam);
        break;
    }
    case WM_UNICHAR:
    {
        if (wParam == UNICODE_NOCHAR)
        {
            return TRUE;
        }

        thisWindow->input.ProcessKeyTyped((int32)wParam);
        break;
    }
    case WM_KEYUP:
    {
        int32 key = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
        thisWindow->input.ProcessKeyUp(key);
        break;
    }
    case WM_KEYDOWN:
    {
        int32 key = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
        thisWindow->input.ProcessKeyDown(key);
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
        const int32 x = GET_X_LPARAM(lParam);
        const int32 y = GET_Y_LPARAM(lParam);

        int button;
        bool down = false;

        if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP)
            button = CT_BUTTON_LEFT;
        else if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
            button = CT_BUTTON_RIGHT;
        else if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
            button = CT_BUTTON_MIDDLE;
        else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
            button = CT_BUTTON_3;
        else
            button = CT_BUTTON_4;

        if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN ||
            message == WM_MBUTTONDOWN || message == WM_XBUTTONDOWN)
        {
            down = true;
        }

        // for (i = 0;  i <= GLFW_MOUSE_BUTTON_LAST;  i++)
        // {
        //     if (window->mouseButtons[i] == GLFW_PRESS)
        //         break;
        // }

        // if (i > GLFW_MOUSE_BUTTON_LAST)
        //     SetCapture(hWnd);

        if (down)
            thisWindow->input.ProcessTouchDown(x, y, button);
        else
            thisWindow->input.ProcessTouchUp(x, y, button);

        // for (i = 0;  i <= GLFW_MOUSE_BUTTON_LAST;  i++)
        // {
        //     if (window->mouseButtons[i] == GLFW_PRESS)
        //         break;
        // }

        // if (i > GLFW_MOUSE_BUTTON_LAST)
        //     ReleaseCapture();

        if (message == WM_XBUTTONDOWN || message == WM_XBUTTONUP)
            return TRUE;

        break;
    }
    case WM_MOUSEMOVE:
    {
        const int32 x = GET_X_LPARAM(lParam);
        const int32 y = GET_Y_LPARAM(lParam);
        thisWindow->input.ProcessMouseMoved(x, y);
        break;
    }
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    {
        thisWindow->input.ProcessMouseScrolled((int16)HIWORD(wParam) / WHEEL_DELTA);
        break;
    }
    case WM_SIZE:
    {
        const int32 width = LOWORD(lParam);
        const int32 height = HIWORD(lParam);
        thisWindow->ProcessWindowResize(width, height);
        break;
    }
    case WM_SETFOCUS:
    {
        thisWindow->ProcessWindowFocus(true);
        break;
    }
    case WM_KILLFOCUS:
    {
        thisWindow->ProcessWindowFocus(false);
        break;
    }
    case WM_DROPFILES:
    {
        HDROP drop = (HDROP)wParam;
        const int32 count = DragQueryFileW(drop, 0xffffffff, NULL, 0);
        Array<String> paths(count);
        WCHAR buffer[MAX_PATH];

        for (int32 i = 0; i < count; ++i)
        {
            DragQueryFileW(drop, i, buffer, MAX_PATH);
            paths.Add(buffer);
        }
        thisWindow->ProcessFilesDropped(std::move(paths));
        DragFinish(drop);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        gApp->RequestQuit();
        break;
    }
    default:
        result = DefWindowProc(hWnd, message, wParam, lParam);
    }

    return result;
}

void WindowsWindow::ProcessWindowResize(int32 width, int32 height)
{
    WindowResizedEvent event(width, height);
    windowResizedHandler(event);
}

void WindowsWindow::ProcessWindowFocus(bool focused)
{
    if (focused)
    {
        FocusGainedEvent event{};
        focusGainedHandler(event);
    }
    else
    {
        FocusLostEvent event{};
        focusLostHandler(event);
    }

    if (!focused)
    {
        input.RelasePressed();
    }
}

void WindowsWindow::ProcessFilesDropped(Array<String> &&paths)
{
    FilesDroppedEvent event(std::forward<Array<String>>(paths));
    fileDroppedHandler(event);
}

uint32 WindowsWindow::GetPositionX() const
{
    POINT pos = {0, 0};
    ClientToScreen(hwnd, &pos);
    return pos.x;
}

uint32 WindowsWindow::GetPositionY() const
{
    POINT pos = {0, 0};
    ClientToScreen(hwnd, &pos);
    return pos.y;
}

uint32 WindowsWindow::GetWidth() const
{
    RECT area;
    GetClientRect(hwnd, &area);
    return area.right;
}

uint32 WindowsWindow::GetHeight() const
{
    RECT area;
    GetClientRect(hwnd, &area);
    return area.bottom;
}

void WindowsWindow::SetTitle(const String &title)
{
    SetWindowTextW(hwnd, *title);
}

void WindowsWindow::Minimize()
{
    ShowWindow(hwnd, SW_MINIMIZE);
}

void WindowsWindow::Maximize()
{
    ShowWindow(hwnd, SW_MAXIMIZE);
}

void WindowsWindow::Restore()
{
    ShowWindow(hwnd, SW_RESTORE);
}

void WindowsWindow::Show()
{
    ShowWindow(hwnd, SW_SHOWNA);
}

void WindowsWindow::Hide()
{
    ShowWindow(hwnd, SW_HIDE);
}

void WindowsWindow::Focus()
{
    BringWindowToTop(hwnd);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

void WindowsWindow::Flash()
{
    FlashWindow(hwnd, TRUE);
}

bool WindowsWindow::Minimized() const
{
    return IsIconic(hwnd);
}

bool WindowsWindow::Maximized() const
{
    return IsZoomed(hwnd);
}

bool WindowsWindow::Visible() const
{
    return IsWindowVisible(hwnd);
}

bool WindowsWindow::Focused() const
{
    return hwnd == GetActiveWindow();
}