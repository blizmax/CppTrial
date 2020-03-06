#include "Application/DebugManager.h"

#if CT_DEBUG

DebugManager debugManager;
DebugManager *gDebugManager = &debugManager;

#include "Application/InputManager.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#if 0
#include "imgui"
#include "Application/Application.h"


static bool g_MouseJustPressed[5] = { false, false, false, false, false };

void ImGuiInit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //=========================Windowing============================

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui::StyleColorsDark();

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "WIP";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = CT_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = CT_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = CT_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = CT_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = CT_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = CT_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = CT_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = CT_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = CT_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = CT_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = CT_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = CT_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = CT_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = CT_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = CT_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = CT_KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = CT_KEY_A;
    io.KeyMap[ImGuiKey_C] = CT_KEY_C;
    io.KeyMap[ImGuiKey_V] = CT_KEY_V;
    io.KeyMap[ImGuiKey_X] = CT_KEY_X;
    io.KeyMap[ImGuiKey_Y] = CT_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = CT_KEY_Z;

    io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
    io.ClipboardUserData = g_Window;
#if defined(_WIN32)
    io.ImeWindowHandle = gApp->GetWindow().GetNativeHandler();
#endif



    //=========================Renderer============================
    io.BackendRendererName = "WIP";

    //TODO 
    //Create Shader
    //CreateFontsTexture


}

void ImGuiCharTyped(char32 c)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(static_cast<uint32>(c));
}

void ImGuiKeyUp(int32 key)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = false;

    // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[CT_KEY_LEFT_CONTROL] || io.KeysDown[CT_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[CT_KEY_LEFT_SHIFT] || io.KeysDown[CT_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[CT_KEY_LEFT_ALT] || io.KeysDown[CT_KEY_RIGHT_ALT];
#ifdef _WIN32
    io.KeySuper = false;
#else
    io.KeySuper = io.KeysDown[CT_KEY_LEFT_SUPER] || io.KeysDown[CT_KEY_RIGHT_SUPER];
#endif
}

void ImGuiKeyDown(int32 key)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = true;

    // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[CT_KEY_LEFT_CONTROL] || io.KeysDown[CT_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[CT_KEY_LEFT_SHIFT] || io.KeysDown[CT_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[CT_KEY_LEFT_ALT] || io.KeysDown[CT_KEY_RIGHT_ALT];
#ifdef _WIN32
    io.KeySuper = false;
#else
    io.KeySuper = io.KeysDown[CT_KEY_LEFT_SUPER] || io.KeysDown[CT_KEY_RIGHT_SUPER];
#endif
}

void ImGuiTouchUp(int32 button)
{

}

void ImGuiTouchDown(int32 button)
{
    ImGuiIO& io = ImGui::GetIO();

    if (button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
        g_MouseJustPressed[button] = true;
}

void ImGuiMouseScrolled(float xoffset, float yoffset)
{
    // TODO
    // ImGuiIO& io = ImGui::GetIO();
    // io.MouseWheelH += xoffset;
    // io.MouseWheel += yoffset;
}

void ImGuiBegin()
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(g_Window, &w, &h);
    glfwGetFramebufferSize(g_Window, &display_w, &display_h);
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step
    double current_time = glfwGetTime();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
    g_Time = current_time;

    // Update buttons
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
        g_MouseJustPressed[i] = false;
    }

     // Update mouse position
    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
#ifdef __EMSCRIPTEN__
    const bool focused = true; // Emscripten
#else
    const bool focused = glfwGetWindowAttrib(g_Window, GLFW_FOCUSED) != 0;
#endif
    if (focused)
    {
        if (io.WantSetMousePos)
        {
            glfwSetCursorPos(g_Window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
        }
        else
        {
            double mouse_x, mouse_y;
            glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
            io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
        }
    }

    ImGui::NewFrame();

    //TODO
    //Draw Content
    
}

void ImGuiEnd()
{
    ImGui::Render();
    
    //TODO

    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    const float ortho_projection[4][4] =
    {
        { 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
        { 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
        { 0.0f,         0.0f,        -1.0f,   0.0f },
        { (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
    };
}

#endif


void DebugManager::OnLoad()
{
#if defined(_WIN32)
    auto &gLog = Logger::GetGlobalLogger();
    gLog.printHandler.Clear();
    gLog.printHandler.On([](LogLevel l, const String &s) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD color = 0;
        switch (l)
        {
        case LogLevel::Debug:
            color = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case LogLevel::Info: 
            color = FOREGROUND_GREEN;
            break;
        case LogLevel::Warning:
            color = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        case LogLevel::Error:
            color = FOREGROUND_RED;
            break;
        default:
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | color);
        std::wcout << s.CStr() << std::endl;
    });
#endif

    gInputManager->mouseMovedEventHandler.On([](InputEvent &event){
        CT_LOG(Debug, event.ToString());
    });

    CT_LOG(Info, CT_TEXT("DebugManager loaded."));
}

void DebugManager::OnUnload()
{

}

void DebugManager::OnUpdate()
{

}

#endif