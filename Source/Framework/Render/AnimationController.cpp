#include "Render/AnimationController.h"
#include "Render/Scene.h"
#include "RenderCore/RenderContext.h"

namespace
{
const String WORLD_MATRIX_BUFFER_NAME = CT_TEXT("WorldMatrixBuffer");
const String INVERSE_TRANSPOSE_WORLD_MATRIX_BUFFER_NAME = CT_TEXT("InverseTransposeWorldMatrixBuffer");

}

UPtr<AnimationController> AnimationController::Create(Scene *scene)
{
    return Memory::MakeUnique<AnimationController>(scene);
}

AnimationController::AnimationController(Scene *scene)
    : scene(scene)
{
    int32 matCount = scene->nodes.Count();
    localMatrices.AddUninitialized(matCount);
    globalMatrices.AddUninitialized(matCount);
    invTransposeGlobalMatrices.AddUninitialized(matCount);
    matricesChanged.AddUninitialized(matCount);

    int32 vecCount = 4 * matCount;
    worldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), vecCount);
    prevWorldMatrixBuffer = worldMatrixBuffer;
    invTransposeWorldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), vecCount);
}

void AnimationController::AddAnimation(int32 meshID, const SPtr<Animation> &anim)
{
    if (!meshes.Contains(meshID))
    {
        meshes.Put(meshID, MeshAnimation());
    }
    meshes[meshID].animations.Add(anim);
}

void AnimationController::ToggleAnimations(bool animate)
{
    for (auto &[k, v] : meshes)
    {
        v.activeAnimation = animate ? 0 : -1;
    }

    //TODO update activeAnimationCount
}

bool AnimationController::Animate(RenderContext *ctx, Animation::AnimTimeType currentTime)
{
    for (auto &v : matricesChanged)
        v = false;

    if (animationChanged == false)
    {
        if (activeAnimationCount == 0)
            return false;

        if (lastAnimationTime == currentTime)
        {
            ctx->CopyResource(prevWorldMatrixBuffer.get(), worldMatrixBuffer.get());
            return false;
        }
    }
    else
    {
        InitLocalMatrices();
    }

    animationChanged = false;
    lastAnimationTime = currentTime;

    for (const auto &[i, m] : meshes)
    {
        if (m.activeAnimation == -1)
            continue;
        auto &anim = m.animations[m.activeAnimation];
        anim->Animate(currentTime, localMatrices);
        for (int32 i = 0; i < anim->GetChannelCount(); ++i)
        {
            matricesChanged[anim->GetChannelMatrixID(i)] = true;
        }
    }

    std::swap(prevWorldMatrixBuffer, worldMatrixBuffer);
    UpdateMatrices();
    BindBuffers();
    ExecuteSkinningPass(ctx);

    return true;
}

int32 AnimationController::GetAnimationCount(int32 meshID) const
{
    auto ptr = meshes.TryGet(meshID);
    return (!ptr) ? 0 : ptr->animations.Count();
}

bool AnimationController::SetActiveAnimationID(int32 meshID, int32 animID)
{
    auto ptr = meshes.TryGet(meshID);
    if (!ptr)
        return false;

    if (ptr->animations.Count() <= animID)
        return false;

    if (ptr->activeAnimation != animID)
    {
        animationChanged = true;

        int32 oldActiveID = ptr->activeAnimation;
        ptr->activeAnimation = animID;

        if (oldActiveID == -1)
        {
            activeAnimationCount++;
        }
        else if (animID == -1)
        {
            activeAnimationCount--;
        }
        AllocPrevWorldMatrixBuffer();
    }

    return true;
}

int32 AnimationController::GetActiveAnimationID(int32 meshID) const
{
    auto ptr = meshes.TryGet(meshID);
    if (!ptr)
        return -1;

    return ptr->activeAnimation;
}

SPtr<Animation> AnimationController::GetAnimation(int32 meshID, int32 animID) const
{
    auto ptr = meshes.TryGet(meshID);
    if (!ptr)
        return nullptr;

    if (animID < 0 || animID >= ptr->animations.Count())
        return nullptr;

    return ptr->animations[animID];
}

SPtr<Animation> AnimationController::GetActiveAnimation(int32 meshID) const
{
    auto ptr = meshes.TryGet(meshID);
    if (!ptr)
        return nullptr;

    int32 animID = ptr->activeAnimation;
    if (animID < 0 || animID >= ptr->animations.Count())
        return nullptr;

    return ptr->animations[animID];
}

void AnimationController::AllocPrevWorldMatrixBuffer()
{
    if (activeAnimationCount > 0)
    {
        if (worldMatrixBuffer == prevWorldMatrixBuffer)
        {
            prevWorldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), worldMatrixBuffer->GetElementCount());
        }
    }
    else
    {
        prevWorldMatrixBuffer = worldMatrixBuffer;
    }
}

void AnimationController::InitLocalMatrices()
{
    for (int32 i = 0; i < localMatrices.Count(); ++i)
    {
        localMatrices[i] = scene->nodes[i].transform;
    }
}

void AnimationController::UpdateMatrices()
{
    globalMatrices = localMatrices;

    auto &nodes = scene->nodes;
    for (int32 i = 0; i < globalMatrices.Count(); ++i)
    {
        if (nodes[i].parent != -1)
        {
            globalMatrices[i] = globalMatrices[nodes[i].parent] * globalMatrices[i];
            matricesChanged[i] = matricesChanged[i] || matricesChanged[nodes[i].parent];
        }

        invTransposeGlobalMatrices[i] = globalMatrices[i].Inverse().Transpose();

        if (skinningPass)
        {
            skinningMatrices[i] = globalMatrices[i] * nodes[i].localToBindPose;
            invTransposeSkinningMatrices[i] = skinningMatrices[i].Inverse().Transpose();
        }
    }
    worldMatrixBuffer->SetBlob(globalMatrices.GetData(), 0, worldMatrixBuffer->GetSize());
    invTransposeWorldMatrixBuffer->SetBlob(invTransposeGlobalMatrices.GetData(), 0, invTransposeWorldMatrixBuffer->GetSize());
}

void AnimationController::BindBuffers()
{
    scene->sceneBlock->SetBuffer(WORLD_MATRIX_BUFFER_NAME, worldMatrixBuffer);
    scene->sceneBlock->SetBuffer(INVERSE_TRANSPOSE_WORLD_MATRIX_BUFFER_NAME, invTransposeWorldMatrixBuffer);
    //TODO sceneBlock prevWorldMatrixBuffer
}

void AnimationController::CreateSkinningPass(const Array<StaticVertexData> &staticDatas, const Array<DynamicVertexData> &dynamicDatas)
{
    if (dynamicDatas.IsEmpty())
        return;

    int32 matCount = scene->nodes.Count();
    skinningMatrices.AddUninitialized(matCount);
    invTransposeSkinningMatrices.AddUninitialized(matCount);

    skinningPass = ComputePass::Create(CT_TEXT("Assets/Shaders/Scene/Skinning.glsl"));
    auto var = skinningPass->GetRootVar();
    var["SkinnedVertexBuffer"] = scene->GetStaticVertexBuffer();
    var["PrevSkinnedVertexBuffer"] = scene->GetPrevVertexBuffer();

    ResourceBindFlags bindFlags = ResourceBind::ShaderResource | ResourceBind::UnorderedAccess;
    auto staticBuffer = Buffer::CreateStructured(sizeof(StaticVertexData), staticDatas.Count(), bindFlags);
    auto dynamiBuffer = Buffer::CreateStructured(sizeof(StaticVertexData), dynamicDatas.Count(), bindFlags);
    staticBuffer->SetBlob(staticDatas.GetData(), 0, staticBuffer->GetSize());
    dynamiBuffer->SetBlob(dynamicDatas.GetData(), 0, dynamiBuffer->GetSize());
    var["StaticVertexBuffer"] = staticBuffer;
    var["DynamicVertexBuffer"] = dynamiBuffer;

    int32 vecCount = 4 * matCount;
    skinningMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), vecCount);
    invTransposeSkinningMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), vecCount);
    var["BoneMatrixBuffer"] = skinningMatrixBuffer;
    var["InverseTransposeBoneMatrixBuffer"] = invTransposeSkinningMatrixBuffer;
    var["WorldMatrixBuffer"] = worldMatrixBuffer;
    var["InverseTransposeBoneMatrixBuffer"] = invTransposeWorldMatrixBuffer;

    skinningDispatchSize = dynamicDatas.Count();
}

void AnimationController::ExecuteSkinningPass(RenderContext *ctx)
{
    if (!skinningPass)
        return;
    
    skinningMatrixBuffer->SetBlob(skinningMatrices.GetData(), 0, skinningMatrixBuffer->GetSize());
    invTransposeSkinningMatrixBuffer->SetBlob(invTransposeSkinningMatrices.GetData(), 0, invTransposeSkinningMatrixBuffer->GetSize());
    skinningPass->Execute(ctx, skinningDispatchSize, 1, 1);
}