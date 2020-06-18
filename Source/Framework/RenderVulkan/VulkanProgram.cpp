#include "RenderVulkan/VulkanProgram.h"
#include "RenderVulkan/VulkanDevice.h"
#include "RenderVulkan/Private/VulkanShaderCompiler.h"
#include "IO/FileHandle.h"

static ShaderType GetShaderType(const String &str)
{
    if (str.StartsWith(CT_TEXT("#type vertex")))
        return ShaderType::Vertex;
    if (str.StartsWith(CT_TEXT("#type pixel")) || str.StartsWith(CT_TEXT("#type fragment")))
        return ShaderType::Pixel;

    CT_EXCEPTION(RenderCore, "Not implement.");
    return ShaderType::Vertex;
}

SPtr<Program> Program::Create(const String &path)
{
    ProgramDesc desc;

    IO::FileHandle file(path);
    if (!file.IsFile())
    {
        CT_EXCEPTION(RenderCore, "Cannot find source file.");
        return nullptr;
    }

    String src = file.ReadString();
    auto lines = src.Split(CT_TEXT("\n"));
    int32 globalLineNum = 0;
    String globalStr;
    int32 startLine = 1;

    for (int32 l = startLine; l <= lines.Count(); ++l)
    {
        auto &line = lines[l - 1];
        if (!line.StartsWith(CT_TEXT("#type ")))
        {
            globalStr += line;
            globalStr += CT_TEXT("\n");
            globalLineNum++;
        }
        else
        {
            startLine = l;
            break;
        }
    }

    auto AppendShaderDesc = [&]() {
        String shaderSrc = globalStr;
        ShaderType shaderType = GetShaderType(lines[startLine - 1]);
        shaderSrc += String::Format(CT_TEXT("#line {0}"), startLine + 1);

        bool eof = true;
        for (int32 l = startLine + 1; l <= lines.Count(); ++l)
        {
            auto &line = lines[l - 1];
            if (!line.StartsWith(CT_TEXT("#type ")))
            {
                shaderSrc += CT_TEXT("\n");
                shaderSrc += line;
            }
            else
            {
                startLine = l;
                eof = false;
                break;
            }
        }
        desc.shaderDescs.Add({shaderType, shaderSrc});
        return !eof;
    };

    while(AppendShaderDesc());

    return Program::Create(desc);
}

SPtr<ProgramKernel> ProgramKernel::Create(const ProgramDesc &desc)
{
    return Memory::MakeShared<VulkanProgramKernel>(desc);
}

VulkanProgramKernel::VulkanProgramKernel(const ProgramDesc &desc) : ProgramKernel(desc)
{
    auto CreateShaderModule = [this](ShaderType shaderType, const Array<uchar8>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.Count();
        createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), &createInfo, gVulkanAlloc, &shaderModule) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create shader module failed.");

        this->shaderDatas.Add({shaderType, shaderModule});
    };

    ProgramReflectionBuilder builder;
    gVulkanShaderCompiler->Compile(desc, CreateShaderModule, builder);
    builder.Build();

    reflection = builder.GetReflection();
    rootSignature = RootSignature::Create(reflection->GetRootSignatureDesc());
}

VulkanProgramKernel::~VulkanProgramKernel()
{
    for (const auto &d : shaderDatas)
    {
        if(gVulkanDevice)
        {
            gVulkanDevice->Release([module = d.module]() {
                vkDestroyShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), module, gVulkanAlloc);
            });
        }
    }
    shaderDatas.Clear();
}