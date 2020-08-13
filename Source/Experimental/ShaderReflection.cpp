#include "Application/Application.h"
#include "IO/FileHandle.h"
#include "Render/RenderManager.h"
#include "RenderCore/RenderAPI.h"

class ShaderReflection : public Logic
{
private:
    SPtr<Program> program;

public:
    virtual void Startup() override
    {
        ProgramCompileOptions options;
        options.generateDebugInfo = true;
        options.reflectAllVariables = true;
        Program::SetCompileOptions(options);

        ProgramDefines defines;
        defines.Put(CT_TEXT("MATERIAL_COUNT"), CT_TEXT("5"));
        //program = Program::Create(CT_TEXT("Assets/Shaders/Experimental/Reflection.glsl"), defines);
        //program = Program::Create(CT_TEXT("Assets/Shaders/Scene/SceneBlock.glsl"), defines);
        program = Program::Create(CT_TEXT("Assets/Shaders/Experimental/FullScreenTest.glsl"));
    }

    virtual void Shutdown() override
    {
    }

    virtual void Tick() override
    {
    }
};

ShaderReflection logic;
Logic *gLogic = &logic;