#include "Application/Desktop/DesktopWindow.h"

void DesktopWindow::CreateNativeWindow(const WindowDesc &desc)
{
    auto title = StringEncode::UTF8::ToChars(desc.title);
    int32 width = desc.width;
    int32 height = desc.height;

    if(desc.fullScreen)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
    }

    window = glfwCreateWindow(width, height, title.GetData(), nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int32 width, int32 height)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        WindowResizedEvent event(width, height);
        thisWindow->windowResizedHandler(event);
    });

    glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int32 focused)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        if (focused)
        {
            FocusGainedEvent event{};
            thisWindow->focusGainedHandler(event);
        }
        else
        {
            FocusLostEvent event{};
            thisWindow->focusLostHandler(event);
        }

        if (!focused)
        {
            thisWindow->input.RelasePressed();
        }
    });

    glfwSetDropCallback(window, [](GLFWwindow* window, int32 count, const char8** p)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));

        Array<String> paths(count);
        for (int32 i = 0; i < count; ++i)
            paths.Add(p[i]);
        FilesDroppedEvent event(std::move(paths));
        thisWindow->fileDroppedHandler(event);
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        thisWindow->shouldClose = true;
    });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
            case GLFW_PRESS:
            case GLFW_REPEAT:
            {
                thisWindow->input.ProcessKeyDown(key);
                break;
            }
            case GLFW_RELEASE:
            {
                thisWindow->input.ProcessKeyUp(key);
                break;
            }
        }
    });

    glfwSetCharCallback(window, [](GLFWwindow* window, uint32 keycode)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        thisWindow->input.ProcessKeyTyped((int32)keycode);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int32 button, int32 action, int32 mods)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        int32 x, y;
        glfwGetWindowPos(window, &x, &y);

        switch (action)
        {
            case GLFW_PRESS:
            {
                thisWindow->input.ProcessTouchDown(x, y, button);
                break;
            }
            case GLFW_RELEASE:
            {
                thisWindow->input.ProcessTouchUp(x, y, button);
                break;
            }
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        thisWindow->input.ProcessMouseScrolled((int32)y);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
    {
        auto thisWindow = reinterpret_cast<DesktopWindow*>(glfwGetWindowUserPointer(window));
        thisWindow->input.ProcessMouseMoved((int32)x, (int32)y);
    });
}

void DesktopWindow::Startup()
{
}

void DesktopWindow::Shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void DesktopWindow::Tick()
{
    glfwPollEvents();
}

int32 DesktopWindow::GetPositionX() const
{
    int32 x, y;
    glfwGetWindowPos(window, &x, &y);
    return x;
}

int32 DesktopWindow::GetPositionY() const
{
    int32 x, y;
    glfwGetWindowPos(window, &x, &y);
    return y;
}

int32 DesktopWindow::GetWidth() const
{
    int32 w, h;
    glfwGetWindowSize(window, &w, &h);
    return w;
}

int32 DesktopWindow::GetHeight() const
{
    int32 w, h;
    glfwGetWindowSize(window, &w, &h);
    return h;
}

void DesktopWindow::SetTitle(const String &title)
{
    glfwSetWindowTitle(window, StringEncode::UTF8::ToChars(title).GetData());
}

void DesktopWindow::Minimize()
{
    glfwIconifyWindow(window);
}

void DesktopWindow::Maximize()
{
    glfwMaximizeWindow(window);
}

void DesktopWindow::Restore()
{
    glfwRestoreWindow(window);
}

void DesktopWindow::Show()
{
    glfwShowWindow(window);
}

void DesktopWindow::Hide()
{
    glfwHideWindow(window);
}

void DesktopWindow::Focus()
{
    glfwFocusWindow(window);
}

void DesktopWindow::Flash()
{
    glfwRequestWindowAttention(window);
}

bool DesktopWindow::Minimized() const
{
    return glfwGetWindowAttrib(window, GLFW_ICONIFIED);
}

bool DesktopWindow::Maximized() const
{
    return glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
}

bool DesktopWindow::Visible() const
{
    return glfwGetWindowAttrib(window, GLFW_VISIBLE);
}

bool DesktopWindow::Focused() const
{
    return glfwGetWindowAttrib(window, GLFW_FOCUSED);
}
