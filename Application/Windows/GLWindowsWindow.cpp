#include "Application/Windows/GLWindowsWindow.h"
#include "glad/glad_wgl.h"
#include <tchar.h>

Window *Window::Create(const WindowConfig &config)
{
    Window *window = new GLWindowsWindow();
    window->CreateNativeWindow(config);
    return window;
}

static LRESULT CALLBACK TempWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uiMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, uiMsg, wParam, lParam);
    }

    return 0;
}

void GLWindowsWindow::CreateNativeWindow(const WindowConfig &config)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = TempWindowProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = _T("GLWindowsWindow");

    RegisterClassEx(&wc);

    DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    HWND TemphWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        wc.lpszClassName,
        _T("NoTitle"),
        Style,
        0,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    HDC TemphDC = GetDC(TemphWnd);
    // Set a temporary default pixel format.
    pixelFormat = ChoosePixelFormat(TemphDC, &pfd);
    if (pixelFormat == 0)
    {
        CT_EXCEPTION(Init, "ChoosePixelFormat failed!");
    }

    if (SetPixelFormat(TemphDC, pixelFormat, &pfd) != 1)
    {
        CT_EXCEPTION(Init, "SetPixelFormat failed!");
    }

    // Create a temporary rendering context.
    HGLRC TempRC = wglCreateContext(TemphDC);
    if (!TempRC)
    {
        CT_EXCEPTION(Init, "wglCreateContext failed!");
    }

    // Set the temporary rendering context as the current rendering context for this window.
    if (wglMakeCurrent(TemphDC, TempRC) != 1)
    {
        CT_EXCEPTION(Init, "wglMakeCurrent failed!");
    }

    if (!gladLoadWGL(TemphDC))
    {
        CT_EXCEPTION(Init, "gladLoadWGL failed!");
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(TempRC);
    ReleaseDC(TemphWnd, TemphDC);
    DestroyWindow(TemphWnd);

    WindowsWindow::CreateNativeWindow(config);

    hdc = GetDC(hwnd);

    // now we try to init OpenGL Core profile context
    if (GLAD_WGL_ARB_pixel_format && GLAD_WGL_ARB_multisample && GLAD_WGL_ARB_create_context)
    {
        // enable MSAA
        const int attributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 24,
            WGL_RED_BITS_ARB, 8,
            WGL_GREEN_BITS_ARB, 8,
            WGL_BLUE_BITS_ARB, 8,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            WGL_SAMPLE_BUFFERS_ARB, 1, // 4x MSAA
            WGL_SAMPLES_ARB, 4,        // 4x MSAA
            0};

        UINT numFormats;

        if (FAILED(wglChoosePixelFormatARB(hdc, attributes, nullptr, 1, &pixelFormat, &numFormats)))
        {
            CT_EXCEPTION(Init, "wglChoosePixelFormatARB failed!");
        }

        if (SetPixelFormat(hdc, pixelFormat, &pfd) != 1)
        {
            CT_EXCEPTION(Init, "SetPixelFormat failed!");
        }

        const int context_attributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0};

        renderContext = wglCreateContextAttribsARB(hdc, 0, context_attributes);
        if (!renderContext)
        {
            CT_EXCEPTION(Init, "wglCreateContextAttributeARB failed!");
        }

        if (wglMakeCurrent(hdc, renderContext) != 1)
        {
            CT_EXCEPTION(Init, "wglMakeCurrent failed!");
        }
    }
    else
    {
        // Set pixel format.
        int pixelFormat = ChoosePixelFormat(hdc, &pfd);
        if (pixelFormat == 0)
        {
            CT_EXCEPTION(Init, "ChoosePixelFormat failed!");
        }

        if (SetPixelFormat(hdc, pixelFormat, &pfd) != 1)
        {
            CT_EXCEPTION(Init, "SetPixelFormat failed!");
        }

        // Create rendering context.
        renderContext = wglCreateContext(hdc);
        if (!renderContext)
        {
            CT_EXCEPTION(Init, "wglCreateContext failed!");
        }

        // Set the rendering context as the current rendering context for this window.
        if (wglMakeCurrent(hdc, renderContext) != 1)
        {
            CT_EXCEPTION(Init, "wglMakeCurrent failed!");
        }
    }
}

void GLWindowsWindow::OnUnload()
{
    if (renderContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(renderContext);
        renderContext = 0;
    }

    WindowsWindow::OnUnload();
}

void GLWindowsWindow::OnTick()
{
    WindowsWindow::OnTick();

    SwapBuffers(hdc);
}