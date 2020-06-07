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

static SPtr<ReflectionVar> ParseSamplerVar(const String &name, const glslang::TObjectReflection &obj)
{
    SPtr<ReflectionVar> result;

    const auto ttype = obj.getType();
    const auto &sampler = ttype->getSampler();
    const auto &qualifier = ttype->getQualifier();

    CT_CHECK(qualifier.hasBinding());
    CT_CHECK(!sampler.isCombined());

    ShaderResourceType shaderResourceType = ShaderResourceType::Unknown;
    ShaderAccess shaderAccess = ShaderAccess::Undefined;
    if (sampler.isPureSampler())
    {
        shaderResourceType = ShaderResourceType::Sampler;
        shaderAccess = ShaderAccess::Read;
    }
    else if (sampler.isTexture())
    {
        bool arrayed = sampler.isArrayed();
        switch (sampler.dim)
        {
        case glslang::Esd1D:
            shaderResourceType = arrayed ? ShaderResourceType::Texture1DArray : ShaderResourceType::Texture1D;
            break;
        case glslang::Esd2D:
            if (sampler.isMultiSample())
                shaderResourceType = arrayed ? ShaderResourceType::Texture2DMSArray : ShaderResourceType::Texture2DMS;
            else
                shaderResourceType = arrayed ? ShaderResourceType::Texture2DArray : ShaderResourceType::Texture2D;
            break;
        case glslang::Esd3D:
            shaderResourceType = ShaderResourceType::Texture3D;
            break;
        case glslang::EsdCube:
            shaderResourceType = arrayed ? ShaderResourceType::TextureCubeArray : ShaderResourceType::TextureCube;
            break;
        case glslang::EsdBuffer:
            //TODO
            break;
        }
        shaderAccess = ShaderAccess::Read;
    }
    else
    {
        //TODO
    }

    if (shaderResourceType == ShaderResourceType::Unknown)
    {
        CT_LOG(Error, CT_TEXT("Parse sampler like var failed, just skip it.{0}:{1}"), name, String(ttype->getCompleteString().c_str()));
    }
    else
    {
        auto resourceType = ReflectionResourceType::Create(shaderResourceType, shaderAccess);
        ShaderVarLocation location;

        if(ttype->isArray())
        {
            auto arraySizes = ttype->getArraySizes();
            CT_CHECK(arraySizes->getNumDims() == 1); //These types only support one dim array

            auto arrayType = ReflectionArrayType::Create(arraySizes->getDimSize(0), 0, resourceType, 0);
            result = ReflectionVar::Create(name, arrayType, location);     
        }
        else
        {
            result = ReflectionVar::Create(name, resourceType, location);
        }
    }

    return result;
}

static SPtr<ReflectionStructType> ParseStruct(const String &name, const glslang::TType *ttype)
{
    uint32 structSize = glslang::TIntermediate::getBlockSize(*ttype);
    SPtr<ReflectionStructType> sturctType = ReflectionStructType::Create(structSize, name);

    const auto &memberTypes = *(ttype->getStruct());
    for (const auto &e : memberTypes)
    {
        //TODO
        auto mtype = e.type;
        CT_LOG(Debug, CT_TEXT("Parent name:{0}, member name:{1}"), name, String(mtype->getFieldName().c_str()));
    }
    return sturctType;
}

static SPtr<ReflectionVar> ParseBlockVar(const String &name, const glslang::TObjectReflection &obj)
{
    SPtr<ReflectionVar> result;

    const auto ttype = obj.getType();
    const auto &qualifier = ttype->getQualifier();

    CT_CHECK(qualifier.hasBinding());

    ShaderResourceType shaderResourceType = ShaderResourceType::Unknown;
    ShaderAccess shaderAccess = ShaderAccess::Undefined;
    if (qualifier.storage == glslang::EvqBuffer)
    {
        CT_LOG(Debug, CT_TEXT("A buffer block, {0}:{1}"), name, String(ttype->getCompleteString().c_str()));

        shaderResourceType = ShaderResourceType::StructuredBuffer;
        shaderAccess = ShaderAccess::ReadWrite;
    }
    else
    {
        CT_LOG(Debug, CT_TEXT("A uniform block, {0}:{1}"), name, String(ttype->getCompleteString().c_str()));
    
        shaderResourceType = ShaderResourceType::ConstantBuffer;
        shaderAccess = ShaderAccess::Read;
    }

    ShaderVarLocation location;
    auto resourceType = ReflectionResourceType::Create(shaderResourceType, shaderAccess);
    result = ReflectionVar::Create(name, resourceType, location);

    if (qualifier.storage == glslang::EvqUniform)
    {
        SPtr<ReflectionType> elementType = ParseStruct(name, ttype);
        auto subBlockReflection = ParameterBlockReflection::Create();
        subBlockReflection->SetElementType(elementType);
        subBlockReflection->Finalize();
        resourceType->SetStructType(elementType);
        resourceType->SetBlockReflection(subBlockReflection);
    }

    return result;
}

static void ParseReflection(const glslang::TProgram &program, const ProgramReflectionBuilder &builder)
{
    auto reflection = builder.GetReflection();
    auto globalBlockReflection = reflection->GetDefaultBlockReflection();
    auto globalStruct = std::static_pointer_cast<ReflectionStructType>(globalBlockReflection->GetElementType());

    for (int32 i = 0; i < program.getNumUniformVariables(); ++i)
    {
        const auto &obj = program.getUniform(i);
        String name = obj.name.c_str();
        const auto ttype = program.getUniformTType(i);
        const auto &qualifier = ttype->getQualifier();
        auto basicType = ttype->getBasicType();

        if (basicType == glslang::EbtSampler) // sampler texture type
		{
            auto var = ParseSamplerVar(name, obj);
            if(var)
            {
                globalStruct->AddMember(var);
                BindingInfo bindingInfo;
                bindingInfo.binding = qualifier.layoutBinding;
                bindingInfo.set = qualifier.layoutSet == glslang::TQualifier::layoutSetEnd ? 0 : qualifier.layoutSet;
                globalBlockReflection->AddBindingInfo(bindingInfo);
            }
        }
        else
        {
            if(qualifier.storage == glslang::EvqUniform || qualifier.storage == glslang::EvqGlobal)
            {
                //uint32 bufferOffset = program.getUniformBufferOffset(i);
            }
            else
            {
                CT_LOG(Debug, CT_TEXT("Tell me this uniform var: {0}"), String(ttype->getCompleteString().c_str()));
            }
        }
    }

    for (int32 i = 0; i < program.getNumUniformBlocks(); ++i)
    {
        const auto &obj = program.getUniformBlock(i);
        String name = obj.name.c_str();
        const auto ttype = program.getUniformTType(i);
        const auto &qualifier = ttype->getQualifier();

        auto var = ParseBlockVar(name, obj);
        if (var)
        {
            globalStruct->AddMember(var);
            BindingInfo bindingInfo;
            bindingInfo.binding = qualifier.layoutBinding;
            bindingInfo.set = qualifier.layoutSet == glslang::TQualifier::layoutSetEnd ? 0 : qualifier.layoutSet;
            globalBlockReflection->AddBindingInfo(bindingInfo);
        }
    }
}

bool VulkanShaderCompilerImpl::Compile(const ProgramDesc &desc, const ShaderModuleFunc &func, const ProgramReflectionBuilder &builder)
{
    glslang::TProgram program;
    Array<UPtr<glslang::TShader>> shaders;

    for (const auto & e : desc.shaderDescs)
    {
        auto u8Str = StringEncode::UTF8::ToChars(e.source);
        char8 *cstr = u8Str.GetData();

        auto stage = ToEShLanguage(e.shaderType);
        shaders.Add(Memory::MakeUnique<glslang::TShader>(stage));
        auto shader = shaders.Last().get();

        shader->setStrings(&cstr, 1);
        shader->setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
        shader->setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
        shader->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

        if (!shader->parse(&DefaultTBuiltInResource, 100, true, EShMsgDefault))
        {
            CT_LOG(Error, CT_TEXT("Parse error: {0}"), String(shader->getInfoLog()));
            CT_EXCEPTION(RenderCore, "Parse failed.");
            return false;
        }

        program.addShader(shader);
    }

    if (!program.link(EShMsgDefault))
    {
        CT_LOG(Error, CT_TEXT("Link error: {0}"), String(program.getInfoLog()));
        CT_EXCEPTION(RenderCore, "Link failed.");
        return false;
    }

    //program.mapIO();
    program.buildReflection();

    CT_LOG(Debug, CT_TEXT("=============================Reflection Begin========================"));
    ParseReflection(program, builder);
    CT_LOG(Debug, CT_TEXT("=============================Reflection End========================"));
    program.dumpReflection();

    glslang::SpvOptions spvOptions;
    spvOptions.generateDebugInfo = false;
    spvOptions.optimizeSize = false;
    spvOptions.disassemble = false;
    spvOptions.validate = false;

    for (const auto &e : desc.shaderDescs)
    {
        std::vector<uint32> spv;
        auto stage = ToEShLanguage(e.shaderType);
        glslang::GlslangToSpv(*program.getIntermediate(stage), spv, nullptr, &spvOptions);

        auto size = 4 * spv.size();
        Array<uchar8> codes;
        codes.AppendUninitialized(size);
        std::memcpy(codes.GetData(), spv.data(), size);

        func(e.shaderType, codes);
    }

    return true;
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