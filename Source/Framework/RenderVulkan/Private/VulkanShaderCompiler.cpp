#define CT_RENDER_CORE_PROGRAM_REFLECTION_IMPLEMENT

#include "RenderVulkan/Private/VulkanShaderCompiler.h"
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <glslang/Public/ShaderLang.h>

/*
layout (std140) uniform ExampleBlock
{
                     // base alignment ----------    // aligned offset
    float value;     // 4                            // 0
    vec3 vector;     // 16                           // 16 (vec4)
    mat4 matrix;     // 16                           // 32 (line 0)
                     // 16                           // 48 (line 1)
                     // 16                           // 64 (line 2)
                     // 16                           // 80 (line 3)
    float values[3]; // 16 (element rounded to vec4) // 96 (values[0])
                     // 16                           // 112 (values[1])
                     // 16                           // 128 (values[2])
    bool boolean;    // 4                            // 144
    int integer;     // 4                            // 148
};
*/

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
    /* .maxDualSourceDrawBuffersEXT = */ 1,

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
    }
};

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
    default:
        CT_EXCEPTION(RenderCore, "Unsupported shader type.");
        return EShLangCount;
    }
}

static SPtr<ReflectionStructType> ParseStructType(const glslang::TType *blockType, const String &name, const glslang::TType *structType, int32 baseOffset);

static SPtr<ReflectionType> ParseDataType(const glslang::TType *blockType, const String &name, const glslang::TType *ttype, int32 totalSize)
{
    ShaderDataType shaderDataType = ShaderDataType::Unknown;
    if (ttype->isVector())
    {
        int32 vectorSize = ttype->getVectorSize();
        switch (ttype->getBasicType())
        {
        case glslang::EbtFloat:
            switch (vectorSize)
            {
            case 2:
                shaderDataType = ShaderDataType::Vec2;
                break;
            case 3:
                shaderDataType = ShaderDataType::Vec3;
                break;
            case 4:
                shaderDataType = ShaderDataType::Vec4;
                break;
            }
            break;
        case glslang::EbtInt:
            switch (vectorSize)
            {
            case 2:
                shaderDataType = ShaderDataType::IVec2;
                break;
            case 3:
                shaderDataType = ShaderDataType::IVec3;
                break;
            case 4:
                shaderDataType = ShaderDataType::IVec4;
                break;
            }
            break;
        case glslang::EbtUint:
            switch (vectorSize)
            {
            case 2:
                shaderDataType = ShaderDataType::UVec2;
                break;
            case 3:
                shaderDataType = ShaderDataType::UVec3;
                break;
            case 4:
                shaderDataType = ShaderDataType::UVec4;
                break;
            }
            break;
        case glslang::EbtBool:
            switch (vectorSize)
            {
            case 2:
                shaderDataType = ShaderDataType::BVec2;
                break;
            case 3:
                shaderDataType = ShaderDataType::BVec3;
                break;
            case 4:
                shaderDataType = ShaderDataType::BVec4;
                break;
            }
            break;
        default:
            break; // error checked later
        }
    }
    else if (ttype->isMatrix())
    {
        switch (ttype->getBasicType())
        {
        case glslang::EbtFloat: //Only support float matrix
            switch (ttype->getMatrixCols())
            {
            case 2:
                switch (ttype->getMatrixRows())
                {
                case 2:
                    shaderDataType = ShaderDataType::Mat2;
                    break;
                case 3:
                    shaderDataType = ShaderDataType::Mat3x2;
                    break;
                case 4:
                    shaderDataType = ShaderDataType::Mat4x2;
                    break;
                }
                break;
            case 3:
                switch (ttype->getMatrixRows())
                {
                case 2:
                    shaderDataType = ShaderDataType::Mat2x3;
                    break;
                case 3:
                    shaderDataType = ShaderDataType::Mat3;
                    break;
                case 4:
                    shaderDataType = ShaderDataType::Mat4x3;
                    break;
                }
                break;
            case 4:
                switch (ttype->getMatrixRows())
                {
                case 2:
                    shaderDataType = ShaderDataType::Mat2x4;
                    break;
                case 3:
                    shaderDataType = ShaderDataType::Mat3x4;
                    break;
                case 4:
                    shaderDataType = ShaderDataType::Mat4;
                    break;
                }
                break;
            }
            break;
        default:
            break; // error checked later
        }
    }
    else
    {
        switch (ttype->getBasicType())
        {
        case glslang::EbtFloat:
            shaderDataType = ShaderDataType::Float;
            break;
        case glslang::EbtInt:
            shaderDataType = ShaderDataType::Int;
            break;
        case glslang::EbtUint:
            shaderDataType = ShaderDataType::UInt;
            break;
        case glslang::EbtBool:
            shaderDataType = ShaderDataType::Bool;
            break;
        default:
            break; // error checked later
        }
    }

    CT_CHECK(shaderDataType != ShaderDataType::Unknown);

    auto reflectionType = ReflectionDataType::Create(shaderDataType);
    reflectionType->SetSize(totalSize);

    return reflectionType;
}

static SPtr<ReflectionArrayType> ParseArrayType(const glslang::TType *blockType, const String &name, const glslang::TType *ttype, int32 totalSize, int32 dim)
{
    SPtr<ReflectionType> elementType;
    auto basicType = ttype->getBasicType();
    auto arraySizes = ttype->getArraySizes();
    uint32 elementCount = arraySizes->getDimSize(dim);
    uint32 stride = totalSize / elementCount;
    int32 nextDim = dim + 1;

    if (arraySizes->getNumDims() > nextDim)
    {
        elementType = ParseArrayType(blockType, name, ttype, stride, nextDim);
    }
    else if (basicType == glslang::EbtStruct)
    {
        elementType = ParseStructType(blockType, name, ttype, 0);
    }
    else
    {
        elementType = ParseDataType(blockType, name, ttype, totalSize);
    }

    auto reflectionType = ReflectionArrayType::Create(elementCount, stride, elementType);
    reflectionType->SetSize(totalSize);

    return reflectionType;
}

static SPtr<ReflectionStructType> ParseStructType(const glslang::TType *blockType, const String &name, const glslang::TType *ttype, int32 baseOffset)
{
    SPtr<ReflectionStructType> structType = ReflectionStructType::Create(name);

    int32 offset = baseOffset;
    int32 totalSize = 0;
    auto &memberTypes = *(ttype->getStruct());
    for (auto &e : memberTypes)
    {
        auto memberType = e.type;
        String memberName = memberType->getFieldName().c_str();
        int32 memberSize = 0;
        glslang::TIntermediate::updateOffset(*blockType, *memberType, offset, memberSize);
        int32 localOffset = offset - baseOffset;
        SPtr<ReflectionType> reflectionType;
        if (memberType->isArray())
        {
            reflectionType = ParseArrayType(blockType, memberName, memberType, memberSize, 0);
        }
        else if (memberType->getBasicType() == glslang::EbtStruct)
        {
            reflectionType = ParseStructType(blockType, memberName, memberType, offset);
        }
        else
        {
            reflectionType = ParseDataType(blockType, memberName, memberType, memberSize);
        }

        offset += memberSize;
        totalSize += memberSize;

        VarLocation location;
        location.byteOffset = localOffset;
        structType->AddMember(ReflectionVar::Create(memberName, reflectionType, location));
    }

    structType->SetSize(totalSize);

    return structType;
}

static SPtr<ReflectionVar> ParseSamplerVar(const String &name, const glslang::TObjectReflection &obj, uint32 rangeIndex)
{
    SPtr<ReflectionVar> result;

    auto ttype = obj.getType();
    auto &sampler = ttype->getSampler();
    auto &qualifier = ttype->getQualifier();

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
        default:
            break; // error checked later
        }
        shaderAccess = ShaderAccess::Read;
    }
    else
    {
    }

    if (shaderResourceType == ShaderResourceType::Unknown)
    {
        CT_LOG(Error, CT_TEXT("Parse unsupported sampler var, ignored.{0}:{1}"), name, String(ttype->getCompleteString().c_str()));
    }
    else
    {
        auto resourceType = ReflectionResourceType::Create(shaderResourceType, shaderAccess);
        VarLocation location;
        location.rangeIndex = rangeIndex;

        if (ttype->isArray())
        {
            auto arraySizes = ttype->getArraySizes();
            CT_CHECK(arraySizes->getNumDims() == 1); //These types only support one dim array

            auto arrayType = ReflectionArrayType::Create(arraySizes->getDimSize(0), 0, resourceType);
            result = ReflectionVar::Create(name, arrayType, location);
        }
        else
        {
            result = ReflectionVar::Create(name, resourceType, location);
        }
    }

    return result;
}

static SPtr<ReflectionVar> ParseBlockVar(const String &name, const glslang::TObjectReflection &obj, uint32 rangeIndex, SPtr<ParameterBlockReflection> &subBlockReflection)
{
    SPtr<ReflectionVar> result;

    auto ttype = obj.getType();
    auto &qualifier = ttype->getQualifier();

    CT_CHECK(qualifier.hasBinding());

    ShaderResourceType shaderResourceType = ShaderResourceType::Unknown;
    ShaderAccess shaderAccess = ShaderAccess::Undefined;
    if (qualifier.storage == glslang::EvqBuffer)
    {
        shaderResourceType = ShaderResourceType::StructuredBuffer;
        shaderAccess = ShaderAccess::ReadWrite;

        // TODO shaderAccess = qualifier.isReadOnly() ? ShaderAccess::Read : ShaderAccess::ReadWrite;
    }
    else
    {
        shaderResourceType = ShaderResourceType::ConstantBuffer;
        shaderAccess = ShaderAccess::Read;
    }

    VarLocation location;
    location.rangeIndex = rangeIndex;
    auto resourceType = ReflectionResourceType::Create(shaderResourceType, shaderAccess);
    result = ReflectionVar::Create(name, resourceType, location);

    if (qualifier.storage == glslang::EvqUniform)
    {
        SPtr<ReflectionType> elementType = ParseStructType(ttype, name, ttype, 0);
        subBlockReflection = ParameterBlockReflection::Create();
        subBlockReflection->SetElementType(elementType);
        subBlockReflection->Finalize();
        resourceType->SetStructType(elementType);
        resourceType->SetBlockReflection(subBlockReflection);
    }

    return result;
}

static void ParseReflection(const glslang::TProgram &program, const ProgramReflectionBuilder &builder, bool printDebugInfo)
{
    auto reflection = builder.GetReflection();
    auto globalBlockReflection = reflection->GetDefaultBlockReflection();
    auto globalStruct = std::static_pointer_cast<ReflectionStructType>(globalBlockReflection->GetElementType());

    for (int32 i = 0; i < program.getNumUniformVariables(); ++i)
    {
        auto &obj = program.getUniform(i);
        String name = obj.name.c_str();
        auto ttype = program.getUniformTType(i);
        auto &qualifier = ttype->getQualifier();
        auto basicType = ttype->getBasicType();

        if (basicType == glslang::EbtSampler) // sampler texture type
        {
            auto var = ParseSamplerVar(name, obj, globalStruct->GetBindingRangeCount());
            if (var)
            {
                globalStruct->AddMember(var);
                BindingInfo bindingInfo;
                bindingInfo.name = name;
                bindingInfo.binding = qualifier.layoutBinding;
                bindingInfo.set = qualifier.layoutSet == glslang::TQualifier::layoutSetEnd ? 0 : qualifier.layoutSet;
                globalBlockReflection->AddBindingInfo(bindingInfo);
            }
        }
        else
        {
            if (qualifier.storage == glslang::EvqUniform || qualifier.storage == glslang::EvqGlobal || qualifier.storage == glslang::EvqBuffer)
            {
            }
            else
            {
                CT_LOG(Debug, CT_TEXT("ParseReflection() Tell me this uniform var: {0}"), String(ttype->getCompleteString().c_str()));
            }
        }
    }

    for (int32 i = 0; i < program.getNumUniformBlocks(); ++i)
    {
        auto &obj = program.getUniformBlock(i);
        String name = obj.name.c_str();
        auto ttype = program.getUniformBlockTType(i);
        auto &qualifier = ttype->getQualifier();

        SPtr<ParameterBlockReflection> subBlockReflection;
        auto var = ParseBlockVar(name, obj, globalStruct->GetBindingRangeCount(), subBlockReflection);
        if (var)
        {
            globalStruct->AddMember(var);
            BindingInfo bindingInfo;
            bindingInfo.name = name;
            bindingInfo.binding = qualifier.layoutBinding;
            bindingInfo.set = qualifier.layoutSet == glslang::TQualifier::layoutSetEnd ? 0 : qualifier.layoutSet;
            globalBlockReflection->AddBindingInfo(bindingInfo);

            if (subBlockReflection)
            {
                subBlockReflection->SetConstantBufferBindingInfo(bindingInfo);
            }
        }
    }

    globalBlockReflection->Finalize();

    if (printDebugInfo)
        CT_LOG(Debug, CT_TEXT("Parse result:{0}"), globalStruct->ToString());
}

bool VulkanShaderCompilerImpl::Compile(const ProgramDesc &desc, const ShaderModuleFunc &func, const ProgramReflectionBuilder &builder)
{
    glslang::TProgram program;
    DirStackFileIncluder includer;
    includer.pushExternalLocalDirectory("Assets/Shaders/");

    auto &options = desc.options;

    String defines;
    for (auto &[k, v] : desc.defines)
        defines += String::Format(CT_TEXT("#define {0} {1}\n"), k, v);
    CT_U8_CSTR_VAR(defines, cstrDefines);

    Array<UPtr<glslang::TShader>> shaders;
    for (auto &e : desc.shaderDescs)
    {
        auto stage = ToEShLanguage(e.shaderType);
        shaders.Add(Memory::MakeUnique<glslang::TShader>(stage));
        auto shader = shaders.Last().get();

        CT_U8_CSTR_VAR(e.source, cstr);
        shader->setStrings(&cstr, 1);
        shader->setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
        shader->setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
        shader->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
        shader->setPreamble(cstrDefines);

        if (!shader->parse(&DefaultTBuiltInResource, 100, true, EShMsgDefault, includer))
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

    int32 ReflectOptions = EShReflectionDefault;
    if (options.reflectAllVariables)
        ReflectOptions |= EShReflectionAllBlockVariables;
    program.buildReflection(ReflectOptions);

    if (options.generateDebugInfo)
    {
        CT_LOG(Debug, CT_TEXT("==========================Reflection========================="));
        ParseReflection(program, builder, true);
        CT_LOG(Debug, CT_TEXT("==========================!Reflection========================="));

        //program.dumpReflection();
    }
    else
    {
        ParseReflection(program, builder, false);
    }

    glslang::SpvOptions spvOptions;
    spvOptions.generateDebugInfo = false;
    spvOptions.optimizeSize = false;
    spvOptions.disassemble = false;
    spvOptions.validate = false;

    for (auto &e : desc.shaderDescs)
    {
        std::vector<uint32> spv;
        auto stage = ToEShLanguage(e.shaderType);
        glslang::GlslangToSpv(*program.getIntermediate(stage), spv, nullptr, &spvOptions);

        auto size = 4 * spv.size();
        Array<uchar8> codes;
        codes.AddUninitialized(size);
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