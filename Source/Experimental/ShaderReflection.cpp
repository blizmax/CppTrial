#include "Core/String.h"
#include "IO/FileHandle.h"
#include "RenderCore/RenderAPI.h"
#include "Application/Application.h"
#include "Render/RenderManager.h"

class ShaderReflection : public Logic
{
private:
    SPtr<Program> program;

public:
    virtual void Startup() override
    {
        ProgramCompileOptions options;
        options.printReflectionInfo = true;
        program = Program::Create(CT_TEXT("Assets/Shaders/Experimental/Reflection.glsl"), {}, options);
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