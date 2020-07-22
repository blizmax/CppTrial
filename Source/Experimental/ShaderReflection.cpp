#include "Application/Application.h"
#include "Core/String.h"
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
        ProgramDefines defines;
        defines.Put(CT_TEXT("MATERIAL_COUNT"), CT_TEXT("5"));
        //program = Program::Create(CT_TEXT("Assets/Shaders/Experimental/Reflection.glsl"), defines, options);
        program = Program::Create(CT_TEXT("Assets/Shaders/Scene/SceneBlock.glsl"), defines, options);
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