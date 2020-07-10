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
        ProgramDesc desc;
        desc.printReflectionInfo = true;
        desc.defines.Put(CT_TEXT("MATERIAL_COUNT"), CT_TEXT("5"));
        IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/Experimental/reflection.frag"));
        desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
        program = Program::Create(desc);
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