#include "RenderVulkan/Private/VulkanShaderCompiler.h"
#include <glslang/public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>

namespace RenderCore
{
const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

void VulkanShaderCompilerImpl::Init()
{
    glslang::InitializeProcess();
}

void VulkanShaderCompilerImpl::Deinit()
{
    glslang::FinalizeProcess();
}

static EShLanguage ToEShLanguage(ShaderType shaderType)
{
    switch (shaderType)
    {
    case ShaderType::Vertex:
        return EShLangVertex;
    case ShaderType::Pixel:
        return EShLangFragment;
    case ShaderType::Geometry:
        return EShLangGeometry;
    case ShaderType::Hull:
        return EShLangTessControl;
    case ShaderType::Domain:
        return EShLangTessEvaluation;
    case ShaderType::Compute:
        return EShLangCompute;
    }
    CT_EXCEPTION(RenderCore, "Unsupported shader type.");
    return EShLangCount;
}

static void ParseUniforms(const glslang::TProgram &program, const SPtr<ProgramReflection> &reflection)
{
    for (int32 i = 0; i < program.getNumLiveUniformVariables(); ++i)
    {
        const char8 *name = program.getUniformName(i);
        const auto ttype = program.getUniformTType(i);
        const auto &qualifier = ttype->getQualifier();
        auto basicType = ttype->getBasicType();

        CT_LOG(Debug, CT_TEXT("{0}"), String(ttype->getCompleteString().c_str()));

        DescriptorType desciptorType = DescriptorType::Unknown;
        uint32 binding = qualifier.layoutBinding;
        uint32 set = qualifier.layoutSet == glslang::TQualifier::layoutSetEnd ? 0 : qualifier.layoutSet;
        uint32 arrayLength = ttype->isArray() ? ttype->getCumulativeArraySize() : 1;

        if (basicType == glslang::EbtSampler) // object type
		{
            CT_CHECK(qualifier.hasBinding());

            const auto &sampler = ttype->getSampler();
            CT_CHECK(!sampler.isCombined());

            if (sampler.isPureSampler())
            {
                desciptorType = DescriptorType::Sampler;
            }
            else if (sampler.isTexture())
            {
                desciptorType = DescriptorType::TextureSrv;
            }
            else
            {
                //TODO image or buffer
                //desciptorType = DescriptorType::TextureUav;
            }
        }
        else
        {
            //Uniform data inside a uniform block.
        }

        for (int32 i = 0; i < program.getNumLiveUniformBlocks(); ++i)
        {
            //TODO
        }
        

        if(desciptorType != DescriptorType::Unknown)
            reflection->AddBindingData(String(name), desciptorType, binding, arrayLength, set);
    }
}

Array<uchar8> VulkanShaderCompilerImpl::Compile(ShaderType shaderType, const String &source, const SPtr<ProgramReflection> &reflection)
{
    auto u8Str = StringEncode::UTF8::ToChars(source);
    char8 *cstr = u8Str.GetData();

    auto stage = ToEShLanguage(shaderType);
    glslang::TShader shader(stage);
    shader.setStrings(&cstr, 1);
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

    if (!shader.parse(&DefaultTBuiltInResource, 100, true, EShMsgDefault))
    {
        CT_LOG(Error, CT_TEXT("Parse error: {0}"), String(shader.getInfoLog()));
        CT_EXCEPTION(RenderCore, "Parse failed.");
    }

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(EShMsgDefault))
    {
        CT_LOG(Error, CT_TEXT("Link error: {0}"), String(program.getInfoLog()));
        CT_EXCEPTION(RenderCore, "Link failed.");
    }

    glslang::SpvOptions spvOptions;
    spvOptions.generateDebugInfo = false;
    spvOptions.optimizeSize = false;
    spvOptions.disassemble = false;
    spvOptions.validate = false;

    program.mapIO();
    program.buildReflection();

    std::vector<uint32> spv;
    glslang::GlslangToSpv(*program.getIntermediate(stage), spv, nullptr, &spvOptions);

    ParseUniforms(program, reflection);

    auto size = 4 * spv.size();
    Array<uchar8> result;
    result.AppendUninitialized(size);
    std::memcpy(result.GetData(), spv.data(), size);
    return result;
}

VulkanShaderCompiler *CreateVulkanShaderCompiler(void)
{
    return Memory::New<VulkanShaderCompilerImpl>();
}

void DestroyVulkanShaderCompiler(VulkanShaderCompiler *compiler)
{
    Memory::Delete(compiler);
}

}