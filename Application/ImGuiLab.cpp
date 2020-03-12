#include "Application/ImGuiLab.h"
#include "Render/RenderAPI.H"
#include "Render/VertexArray.h"
#include "Render/OrthographicCamera.h"

//TEMP
#include "glad/glad.h"

ImGuiLab imguiLab;
ImGuiLab *gImGuiLab = &imguiLab;

void ImGuiLab::OnLoad()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
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

    static bool open = true;
    ImGui::ShowDemoWindow(&open);

    End();
}

void ImGuiLab::BindPlatform()
{
    ImGuiIO &io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
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

    KeyTypedHandle = gApp->GetInput().keyTypedEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<KeyTypedEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter(e.character);
    });

    keyUpHandle = gApp->GetInput().keyUpEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<KeyUpEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
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

    keyDownHandle = gApp->GetInput().keyDownEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<KeyDownEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
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

    touchUpHandle = gApp->GetInput().touchUpEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<TouchUpEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.button] = false;
    });

    touchDownHandle = gApp->GetInput().touchDownEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<TouchDownEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.button] = true;
    });

    mouseMovedHandle = gApp->GetInput().mouseMovedEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<MouseMovedEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2((float)e.x, (float)e.y);
    });

    mouseScrolledHandle = gApp->GetInput().mouseScrolledEventHandler.On([](InputEvent &event) {
        auto &e = static_cast<MouseScrolledEvent &>(event);
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += (float)e.amount;
        //io.MouseWheelH += (float)e.amount;
    });
}

void ImGuiLab::BindRenderer()
{
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "WIP";

    shader = Shader::Create(CT_TEXT("Assets/Shaders/ImGuiShader.glsl"));

    uchar8 *pixels;
    int32 width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    texture = Texture::Create(width, height);
    texture->SetData(static_cast<void *>(pixels), width * height * 4);

    //io.Fonts->TexID = 0;
}

void ImGuiLab::UnbindPlatform()
{
    KeyTypedHandle.Off();
    keyUpHandle.Off();
    keyDownHandle.Off();
    touchUpHandle.Off();
    touchDownHandle.Off();
    mouseMovedHandle.Off();
    mouseScrolledHandle.Off();
}

void ImGuiLab::UnbindRenderer()
{
    shader.reset();
    texture.reset();
}

void ImGuiLab::Begin()
{
    ImGuiIO &io = ImGui::GetIO();
    if (!io.Fonts->IsBuilt())
    {
        CT_EXCEPTION(ImGuiLab, "Font atlas not built!");
        return;
    }

    io.DeltaTime = gApp->GetDeltaTime();

    uint32 width = gApp->GetWindow().GetWidth();
    uint32 height = gApp->GetWindow().GetHeight();
    io.DisplaySize = ImVec2((float)width, (float)height);
    if (width > 0 && height > 0)
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    // WIP
    // UpdateMouseCursor
    // UpdateGamepads

    ImGui::NewFrame();
}

void ImGuiLab::End()
{
    ImGui::Render();

    RenderDrawData(ImGui::GetDrawData());

    // {
    //     SetupRenderState(ImGui::GetDrawData(), gApp->GetWindow().GetWidth(), gApp->GetWindow().GetHeight());

    //     float vertexData[] = {
    //         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    //         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //         0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    //         -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

    //     uint32 indexData[] = {
    //         0, 1, 2,
    //         2, 3, 0};

    //     auto vertexBuffer = VertexBuffer::Create(vertexData, sizeof(vertexData));
    //     vertexBuffer->SetLayout({
    //         {CT_TEXT("position"), VertexDataType::Float3},
    //         {CT_TEXT("texCoord"), VertexDataType::Float2},
    //     });

    //     auto indexBuffer = IndexBuffer::Create(indexData, 6);

    //     auto vertexArray = VertexArray::Create();
    //     vertexArray->AddVertexBuffer(vertexBuffer);
    //     vertexArray->SetIndexBuffer(indexBuffer);

    //     Matrix4 scaleMat = Matrix4::Scale(texture->GetWidth(), texture->GetHeight(), 1.0f);
    //     shader->SetInt(CT_TEXT("mainTex"), 0);

    //     RenderAPI::DrawIndexed(vertexArray);
    // }

    uint32 width = gApp->GetWindow().GetWidth();
    uint32 height = gApp->GetWindow().GetHeight();

    RenderAPI::SetViewport(0, 0, width, height);
    RenderAPI::SetScissor(0, 0, width, height);
}

void ImGuiLab::SetupRenderState(ImDrawData *drawData, uint32 width, uint32 height)
{
    RenderAPI::SetViewport(0, 0, width, height);

    float L = drawData->DisplayPos.x;
    float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    float T = drawData->DisplayPos.y;
    float B = drawData->DisplayPos.y + drawData->DisplaySize.y;

    // Matrix4 mvp{
    //     2.0f / (R - L),
    //     0.0f,
    //     0.0f,
    //     0.0f,
    //     0.0f,
    //     2.0f / (T - B),
    //     0.0f,
    //     0.0f,
    //     0.0f,
    //     0.0f,
    //     -1.0f,
    //     0.0f,
    //     (R + L) / (L - R),
    //     (T + B) / (B - T),
    //     0.0f,
    //     1.0f,
    // };

    OrthographicCamera camera(width, height);
    //camera.position.x = L + (R - L) / 2.0f;
    //camera.position.y = T + (B - T) / 2.0f;
    camera.zoom = 3.0f;
    camera.Update();

    //uint32 tw = texture->GetWidth();
    //uint32 th = texture->GetHeight();
    //Matrix4 mvp = camera.combined * Matrix4::Scale(texture->GetWidth(), texture->GetHeight(), 1.0f);
    Matrix4 mvp = camera.combined;


    shader->Bind();
    texture->Bind(0);
    shader->SetInt(CT_TEXT("mainTex"), 0);
    shader->SetMatrix4(CT_TEXT("mvp"), mvp);
}

void ImGuiLab::RenderDrawData(ImDrawData *drawData)
{
    int32 width = (int32)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
    int32 height = (int32)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
    if (width <= 0 || height <= 0)
        return;

    ImVec2 clip_off = drawData->DisplayPos;
    ImVec2 clip_scale = drawData->FramebufferScale;

    for (int32 n = 0; n < drawData->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = drawData->CmdLists[n];

        //FIXME: GL_STREAM_DRAW
        auto vbo = VertexBuffer::Create((float *)cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
        vbo->SetLayout({
            {CT_TEXT("inPosition"), VertexDataType::Float2},
            {CT_TEXT("inUV"), VertexDataType::Float2},
            {CT_TEXT("inColor"), VertexDataType::UByte4, true},
        });
        auto ebo = IndexBuffer::Create((uint32 *)cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size);
        auto vao = VertexArray::Create();
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ebo);
        vao->Bind();


        //TEST
        CT_LOG(Error, CT_TEXT("SIZE:{0}"), sizeof(ImDrawIdx));
        // for(uint32 i = 0; i < 50; ++i)
        // {
        //     auto v = cmd_list->VtxBuffer.Data[i];
        //     CT_LOG(Error, CT_TEXT("index:{0}, data:{1},{2},{3},{4},{5}"), i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.col);
        // }

        for (int32 cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

            //TEST
            //auto offset = pcmd->IdxOffset;
            //CT_LOG(Error, CT_TEXT("index:{0}, offset:{1}"), cmd_i, offset);

            if (pcmd->UserCallback != NULL)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    SetupRenderState(drawData, width, height);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < width && clip_rect.y < height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    RenderAPI::SetScissor((int32)clip_rect.x, (int32)clip_rect.y, (int32)clip_rect.z, (int32)clip_rect.w);
                    //glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                    //RenderAPI::DrawIndexed(vao);


                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
                }
            }
        }
    }
}
