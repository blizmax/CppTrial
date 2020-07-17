#include "Application/ImGuiLab.h"
#include "Render/OrthographicCamera.h"
#include "Render/RenderManager.h"

ImGuiLab imguiLab;
ImGuiLab *gImGuiLab = &imguiLab;

void ImGuiLab::Startup()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    BindPlatform();
    BindRenderer();
}

void ImGuiLab::Shutdown()
{
    UnbindRenderer();
    UnbindPlatform();

    ImGui::DestroyContext();
}

void ImGuiLab::Tick()
{
    CT_PROFILE_FUNCTION();

    Begin();

    static bool open = false;
    ImGui::ShowDemoWindow(&open);

    drawHandler();

    End();
}

void ImGuiLab::BindPlatform()
{
    ImGuiIO &io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "WIP";

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

    // #if defined(_WIN32)
    //     io.ImeWindowHandle = gApp->GetWindow().GetNativeHandle();
    // #endif

    // TODO: Clipboard

    // WIP: Create mouse cursors

    // Input events

    gApp->GetInput().keyTypedHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.AddInputCharacter(event.character);
        event.handled = io.WantCaptureKeyboard;
    });

    gApp->GetInput().keyUpHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[event.key] = false;
        io.KeyCtrl = io.KeysDown[CT_KEY_LEFT_CONTROL] || io.KeysDown[CT_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[CT_KEY_LEFT_SHIFT] || io.KeysDown[CT_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[CT_KEY_LEFT_ALT] || io.KeysDown[CT_KEY_RIGHT_ALT];
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[CT_KEY_LEFT_SUPER] || io.KeysDown[CT_KEY_RIGHT_SUPER];
#endif
        event.handled = io.WantCaptureKeyboard;
    });

    gApp->GetInput().keyDownHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[event.key] = true;
        io.KeyCtrl = io.KeysDown[CT_KEY_LEFT_CONTROL] || io.KeysDown[CT_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[CT_KEY_LEFT_SHIFT] || io.KeysDown[CT_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[CT_KEY_LEFT_ALT] || io.KeysDown[CT_KEY_RIGHT_ALT];
#ifdef _WIN32
        io.KeySuper = false;
#else
        io.KeySuper = io.KeysDown[CT_KEY_LEFT_SUPER] || io.KeysDown[CT_KEY_RIGHT_SUPER];
#endif
        event.handled = io.WantCaptureKeyboard;
    });

    gApp->GetInput().touchUpHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.button] = false;
        event.handled = io.WantCaptureMouse;
    });

    gApp->GetInput().touchDownHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.button] = true;
        event.handled = io.WantCaptureMouse;
    });

    gApp->GetInput().mouseMovedHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2((float)event.x, (float)event.y);
        event.handled = io.WantCaptureMouse;
    });

    gApp->GetInput().mouseScrolledHandler += ([](auto &event) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += (float)event.amount;
        //io.MouseWheelH += (float)e.amount;
        event.handled = io.WantCaptureMouse;
    });
}

void ImGuiLab::BindRenderer()
{
    ImGuiIO &io = ImGui::GetIO();
    io.BackendRendererName = "WIP";

    program = Program::Create(CT_TEXT("Assets/Shaders/ImGui.glsl"));
    programVars = GraphicsVars::Create(program);
    graphicsState = GraphicsState::Create();
    graphicsState->SetProgram(program);

    auto vertexBufferLayout = VertexBufferLayout::Create(
        { { CT_TEXT("VertexPosition"), ResourceFormat::RG32Float },
          { CT_TEXT("VertexUV"), ResourceFormat::RG32Float },
          { CT_TEXT("VertexColor"), ResourceFormat::RGBA8Unorm } });
    vertexLayout = VertexLayout::Create();
    vertexLayout->AddBufferLayout(vertexBufferLayout);

    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;
    {
        RasterizationStateDesc desc;
        desc.cullMode = CullMode::None;
        desc.scissorEnabled = true;
        desc.depthClampEnabled = false;
        rasterizationState = RasterizationState::Create(desc);
    }
    {
        DepthStencilStateDesc desc;
        desc.depthReadEnabled = false;
        desc.depthWriteEnabled = false;
        depthStencilState = DepthStencilState::Create(desc);
    }
    {
        BlendStateDesc desc;
        desc.attachments.SetCount(1);
        desc.attachments[0].enabled = true;
        desc.attachments[0].srcAlphaFactor = BlendFactor::OneMinusSrcAlpha;
        desc.attachments[0].dstAlphaFactor = BlendFactor::Zero;
        blendState = BlendState::Create(desc);
    }

    graphicsState->SetRasterizationState(rasterizationState);
    graphicsState->SetDepthStencilState(depthStencilState);
    graphicsState->SetBlendState(blendState);

    uchar8 *pixels;
    int32 width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    sampler = Sampler::Create(SamplerDesc());
    texture = Texture::Create2D(width, height, ResourceFormat::RGBA8Unorm, 1, 1, pixels);

    //io.Fonts->TexID = 0;

    programVars->Root()[CT_TEXT("mainSampler")] = sampler;
    programVars->Root()[CT_TEXT("mainTex")] = texture;
}

void ImGuiLab::UnbindPlatform()
{
}

void ImGuiLab::UnbindRenderer()
{
    vertexLayout.reset();
    vertexArray.reset();
    texture.reset();
    sampler.reset();
    programVars.reset();
    graphicsState.reset();
    program.reset();
}

void ImGuiLab::CreateVertexArray(ImDrawData *drawData)
{
    int32 vboSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
    int32 iboSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);

    bool createVbo = true;
    bool createIbo = true;
    if (vertexArray)
    {
        createVbo = vertexArray->GetVertexBuffer(0)->GetSize() <= vboSize;
        createIbo = vertexArray->GetIndexBuffer()->GetSize() <= iboSize;
        if (!createVbo && !createIbo)
            return;
    }

    auto vbo = createVbo ? Buffer::Create(vboSize, ResourceBind::Vertex, CpuAccess::Write) : vertexArray->GetVertexBuffer(0);
    auto ibo = createIbo ? Buffer::Create(iboSize, ResourceBind::Index, CpuAccess::Write) : vertexArray->GetIndexBuffer();

    vertexArray = VertexArray::Create();
    vertexArray->SetVertexLayout(vertexLayout);
    vertexArray->AddVertexBuffer(vbo);
    vertexArray->SetIndexBuffer(ibo);
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

    auto width = gApp->GetWindow().GetWidth();
    auto height = gApp->GetWindow().GetHeight();
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
}

void ImGuiLab::SetupRenderState(ImDrawData *drawData, uint32 width, uint32 height)
{
    Viewport viewport;
    viewport.width = (float)width;
    viewport.height = (float)height;
    graphicsState->SetViewport(0, viewport);

    float L = drawData->DisplayPos.x;
    float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    float T = drawData->DisplayPos.y;
    float B = drawData->DisplayPos.y + drawData->DisplaySize.y;

    Matrix4 mvp = Matrix4::Ortho(L, R, B, T, -1.0f, 1.0f);
    programVars->Root()[CT_TEXT("UB")][CT_TEXT("mvp")] = mvp;
}

void ImGuiLab::RenderDrawData(ImDrawData *drawData)
{
    int32 width = (int32)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
    int32 height = (int32)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
    if (width <= 0 || height <= 0)
        return;

    if (drawData->CmdListsCount == 0)
        return;

    auto ctx = gRenderManager->GetRenderContext();
    auto &fbo = gRenderManager->GetTargetFrameBuffer();
    graphicsState->SetFrameBuffer(fbo);

    CreateVertexArray(drawData);
    graphicsState->SetVertexArray(vertexArray);
    auto vertexPtr = (ImDrawVert *)vertexArray->GetVertexBuffer(0)->Map(BufferMapType::WriteDiscard);
    auto indexPtr = (ImDrawIdx *)vertexArray->GetIndexBuffer()->Map(BufferMapType::WriteDiscard);
    for (int32 n = 0; n < drawData->CmdListsCount; n++)
    {
        const ImDrawList *cmdList = drawData->CmdLists[n];
        std::memcpy(vertexPtr, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
        std::memcpy(indexPtr, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
        vertexPtr += cmdList->VtxBuffer.Size;
        indexPtr += cmdList->IdxBuffer.Size;
    }
    vertexArray->GetVertexBuffer(0)->Unmap();
    vertexArray->GetIndexBuffer()->Unmap();

    SetupRenderState(drawData, width, height);

    ImVec2 clipOff = drawData->DisplayPos;
    ImVec2 clipScale = drawData->FramebufferScale;
    uint32 vertexOffset = 0;
    uint32 indexOffset = 0;
    for (int32 n = 0; n < drawData->CmdListsCount; n++)
    {
        const ImDrawList *cmdList = drawData->CmdLists[n];
        for (int32 i = 0; i < cmdList->CmdBuffer.Size; i++)
        {
            const ImDrawCmd *cmd = &cmdList->CmdBuffer[i];

            if (cmd->UserCallback != NULL)
            {
                if (cmd->UserCallback == ImDrawCallback_ResetRenderState)
                    SetupRenderState(drawData, width, height);
                else
                    cmd->UserCallback(cmdList, cmd);
            }
            else
            {
                ImVec4 clipRect;
                clipRect.x = (cmd->ClipRect.x - clipOff.x) * clipScale.x;
                clipRect.y = (cmd->ClipRect.y - clipOff.y) * clipScale.y;
                clipRect.z = (cmd->ClipRect.z - clipOff.x) * clipScale.x;
                clipRect.w = (cmd->ClipRect.w - clipOff.y) * clipScale.y;
                if (clipRect.x < width && clipRect.y < height && clipRect.z >= 0.0f && clipRect.w >= 0.0f)
                {
                    if (clipRect.x < 0.0f)
                        clipRect.x = 0.0f;
                    if (clipRect.y < 0.0f)
                        clipRect.y = 0.0f;

                    Scissor scissor;
                    scissor.x = (int32)clipRect.x;
                    scissor.y = (int32)(clipRect.y);
                    scissor.width = (uint32)(clipRect.z - clipRect.x);
                    scissor.height = (uint32)(clipRect.w - clipRect.y);
                    graphicsState->SetScissor(0, scissor);

                    ctx->DrawIndexed(graphicsState.get(), programVars.get(), cmd->ElemCount, cmd->IdxOffset + indexOffset, cmd->VtxOffset + vertexOffset);
                }
            }
        }

        vertexOffset += cmdList->VtxBuffer.Size;
        indexOffset += cmdList->IdxBuffer.Size;
    }
}
