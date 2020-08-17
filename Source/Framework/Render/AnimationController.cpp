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
    worldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), 4 * vecCount);
    prevWorldMatrixBuffer = worldMatrixBuffer;
    invTransposeWorldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), 4 * vecCount);
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

    //TODO skinning pass
    return true;
}

int32 AnimationController::GetAnimationCount(int32 meshID) const
{
    auto ptr = meshes.TryGet(meshID);
    return (!ptr) ? 0 : ptr->animations.Count();
}

SPtr<Animation> AnimationController::GetAnimation(int32 meshID, int32 animID) const
{
    auto ptr = meshes.TryGet(meshID);
    if (!ptr)
        return nullptr;

    if (ptr->animations.Count() <= animID)
        return nullptr;

    return ptr->animations[animID];
}

bool AnimationController::SetActiveAnimation(int32 meshID, int32 animID)
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

int32 AnimationController::GetActiveAnimation(int32 meshID) const
{
    auto ptr = meshes.TryGet(meshID);
    if (!ptr)
        return -1;

    return ptr->activeAnimation;
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

        //TODO skinning pass
    }
    worldMatrixBuffer->SetBlob(globalMatrices.GetData(), 0, worldMatrixBuffer->GetSize());
    invTransposeWorldMatrixBuffer->SetBlob(invTransposeGlobalMatrices.GetData(), 0, invTransposeWorldMatrixBuffer->GetSize());
}

void AnimationController::BindBuffers()
{
    scene->sceneBlock->SetBuffer(WORLD_MATRIX_BUFFER_NAME, worldMatrixBuffer);
    scene->sceneBlock->SetBuffer(INVERSE_TRANSPOSE_WORLD_MATRIX_BUFFER_NAME, invTransposeWorldMatrixBuffer);
}

void AnimationController::CreateSkinningPass()
{
    //TODO
}

void AnimationController::ExecuteSkinningPass(RenderContext *ctx)
{
    if (!skinningPass)
        return;
    
    skinningMatrixBuffer->SetBlob(skinningMatrices.GetData(), 0, skinningMatrixBuffer->GetSize());
    invTransposeSkinningMatrixBuffer->SetBlob(invTransposeSkinningMatrices.GetData(), 0, invTransposeSkinningMatrixBuffer->GetSize());
    skinningPass->Execute(ctx, skinningDispatchSize, 1, 1);
}