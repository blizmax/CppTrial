#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RenderOpenGL/Desktop/GLDesktopWindow.h"

Window *Window::Create(const WindowDesc &desc)
{
    Window *window = Memory::New<GLDesktopWindow>();
    window->CreateNativeWindow(desc);
    
    glfwMakeContextCurrent((GLFWwindow*)window->GetNativeHandle());
    return window;
}

void Window::Init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
}

void GLDesktopWindow::CreateNativeWindow(const WindowDesc &desc)
{
    DesktopWindow::CreateNativeWindow(desc);

    glfwMakeContextCurrent(window);
}

void GLDesktopWindow::Tick()
{
    DesktopWindow::Tick();

    glfwSwapBuffers(window);
}
