#include "Application/Windows/WindowsWindow.h"
#include "Application/Application.h"
#include "Application/InputManager.h"
#include <tchar.h>

void WindowsWindow::CreateNativeWindow(const WindowConfig &config)
{
    // get the HINSTANCE of the Console Program
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("WindowsWindow");

    // register the window class
    RegisterClassEx(&wc);

    int height_adjust = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER);
    int width_adjust = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);

    // create the window and use the result as the handle
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

    // display the window on the screen
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
    // this struct holds Windows event messages
    MSG msg;

    // we use PeekMessage instead of GetMessage here
    // because we should not block the thread at anywhere
    // except the engine execution driver module
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);

        // send the message to the WindowProc function
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
        gInputManager->ProcessKeyTyped((int32)wParam);
        break;
    }
    case WM_UNICHAR:
    {
        if (wParam == UNICODE_NOCHAR)
        {
            return TRUE;
        }

        gInputManager->ProcessKeyTyped((int32)wParam);
        break;
    }
    case WM_KEYUP:
    {
        int32 key = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
        gInputManager->ProcessKeyUp(key);
        break;
    }
    case WM_KEYDOWN:
    {
        int32 key = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
        gInputManager->ProcessKeyDown(key);
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
            gInputManager->ProcessTouchDown(button);
        else
            gInputManager->ProcessTouchUp(button);

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
        gInputManager->ProcessMouseMoved(x, y);
        break;
    }
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    {
        gInputManager->ProcessMouseScrolled((int32)HIWORD(wParam));
        break;
    }
    case WM_SIZE:
    {
        //TODO
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