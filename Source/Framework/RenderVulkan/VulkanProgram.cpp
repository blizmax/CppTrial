#include "RenderVulkan/VulkanProgram.h"
#include "IO/FileHandle.h"
#include "RenderVulkan/Private/VulkanShaderCompiler.h"
#include "RenderVulkan/VulkanDevice.h"

static ShaderType GetShaderType(const String &str)
{
    if (str.StartsWith(CT_TEXT("#type vertex")))
        return ShaderType::Vertex;
    if (str.StartsWith(CT_TEXT("#type pixel")) || str.StartsWith(CT_TEXT("#type fragment")))
        return ShaderType::Pixel;
    if (str.StartsWith(CT_TEXT("#type compute")))
        return ShaderType::Compute;

    CT_EXCEPTION(RenderCore, "Not implement.");
    return ShaderType::Vertex;
}

static ShaderLanguage GetShaderLanguage(const String &ext)
{
    if (ext == CT_TEXT(".glsl"))
        return ShaderLanguage::Glsl;
    if (ext == CT_TEXT(".hlsl"))
        return ShaderLanguage::Hlsl;
    return ShaderLanguage::Unknown;
}

static String GetVersionAndExtensions(ShaderLanguage lang)
{
    return lang != ShaderLanguage::Glsl ? CT_TEXT("")
                           : CT_TEXT("#version 450\n"
                                     "#extension GL_ARB_separate_shader_objects : enable\n"
                                     "#extension GL_GOOGLE_include_directive : enable\n");
}

ProgramDesc Program::CreateDesc(const String &path, const ProgramDefines &defines)
{
    ProgramDesc desc;
    desc.defines = defines;
    desc.options = sOptions;

    IO::FileHandle file(path);
    if (!file.IsFile())
    {
        CT_EXCEPTION(RenderCore, "Cannot find source file.");
        return desc;
    }

    ShaderLanguage shaderLang = GetShaderLanguage(file.GetExtension());
    if (shaderLang == ShaderLanguage::Unknown)
    {
        CT_EXCEPTION(RenderCore, "Unsupported shader language.");
        return desc;
    }

    String src = file.ReadString();
    auto lines = src.Split(CT_TEXT("\n"));
    int32 globalLineNum = 0;
    String globalStr = GetVersionAndExtensions(shaderLang);
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
        desc.shaderDescs.Add({ shaderType, shaderLang, shaderSrc });
        return !eof;
    };

    while (AppendShaderDesc())
        ;

    return desc;
}

SPtr<ProgramKernel> ProgramKernel::Create(const ProgramDesc &desc)
{
    return Memory::MakeShared<VulkanProgramKernel>(desc);
}

VulkanProgramKernel::VulkanProgramKernel(const ProgramDesc &desc)
    : ProgramKernel(desc)
{
    auto CreateShaderModule = [this](ShaderType shaderType, const Array<uchar8> &code) {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.Count();
        createInfo.pCode = reinterpret_cast<const uint32 *>(code.GetData());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), &createInfo, gVulkanAlloc, &shaderModule) != VK_SUCCESS)
            CT_EXCEPTION(RenderCore, "Create shader module failed.");

        this->shaderDatas.Add({ shaderType, shaderModule });
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
        if (gVulkanDevice)
        {
            gVulkanDevice->Release([module = d.module]() {
                vkDestroyShaderModule(gVulkanDevice->GetLogicalDeviceHandle(), module, gVulkanAlloc);
            });
        }
    }
    shaderDatas.Clear();
}