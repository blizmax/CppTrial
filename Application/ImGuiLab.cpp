#include "Application/ImGuiLab.h"

ImGuiLab imguiLab;
ImGuiLab *gImGuiLab = &imguiLab;

void ImGuiLab::OnLoad()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    BindPlatform();
    BindRenderer();
}

void ImGuiLab::OnUnload()
{
    UnbindRenderer();
    UnbindPlatform();

    ImGui::DestroyContext();
}

void ImGuiLab::OnUpdate()
{
    Begin();

    //TODO

    End();
}

void ImGuiLab::BindPlatform()
{
    ImGuiIO& io = ImGui::GetIO();
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

#if defined(_WIN32)
    io.ImeWindowHandle = gApp->GetWindow().GetNativeHandler();
#endif

    // TODO: Clipboard

    // WIP: Create mouse cursors


    // Input events
    keyUpHandle = gApp->GetInput().keyUpEventHandler.On([](InputEvent &event){
        auto &e = static_cast<KeyUpEvent&>(event);
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.key] = false;
        io.KeyCtrl = io.KeysDown[CT_KEY_LEFT_CONTROL] || io.KeysDown[CT_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[CT_KEY_LEFT_SHIFT] || io.KeysDown[CT_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[CT_KEY_LEFT_ALT] || io.KeysDown[CT_KEY_RIGHT_ALT];
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[CT_KEY_LEFT_SUPER] || io.KeysDown[CT_KEY_RIGHT_SUPER];
#endif
    });

    keyDownHandle = gApp->GetInput().keyDownEventHandler.On([](InputEvent &event){
        auto &e = static_cast<KeyDownEvent&>(event);
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.key] = true;
        io.KeyCtrl = io.KeysDown[CT_KEY_LEFT_CONTROL] || io.KeysDown[CT_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[CT_KEY_LEFT_SHIFT] || io.KeysDown[CT_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[CT_KEY_LEFT_ALT] || io.KeysDown[CT_KEY_RIGHT_ALT];
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[CT_KEY_LEFT_SUPER] || io.KeysDown[CT_KEY_RIGHT_SUPER];
#endif
    });
}

void ImGuiLab::BindRenderer()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "WIP";

    shader = Shader::Create(CT_TEXT("Assets/Shaders/ImGuiShader.glsl"));

    uchar8* pixels;
    int32 width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    texture = Texture::Create(width, height);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->SetData(static_cast<void*>(pixels), width * height * 4);

    //io.Fonts->TexID = 0;
}

void ImGuiLab::UnbindPlatform()
{
    keyUpHandle.Off();
    keyDownHandle.Off();
}

void ImGuiLab::UnbindRenderer()
{

}

void ImGuiLab::Begin()
{
    //TODO UpdatePlatform

    ImGui::NewFrame();
}

void ImGuiLab::End()
{
    ImGui::Render();

    //TODO RenderDrawData(ImGui::GetDrawData());
}