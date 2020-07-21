#include "Render/AnimationController.h"
#include "Render/Scene.h"
#include "RenderCore/RenderContext.h"

AnimationController::AnimationController(Scene *scene)
    : scene(scene)
{
    int32 matCount = scene->nodes.Count();
    localMatrices.SetCount(matCount);
    globalMatrices.SetCount(matCount);
    invTransposeGlobalMatrices.SetCount(matCount);

    CreateBuffers();
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
    //TODO
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
        else if(animID == -1)
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

void AnimationController::CreateBuffers()
{
    int32 count = 4 * scene->nodes.Count();

    worldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), 4 * count,  ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);
    prevWorldMatrixBuffer = worldMatrixBuffer;
    invTransposeWorldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), 4 * count,  ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);

    //TODO
}

void AnimationController::AllocPrevWorldMatrixBuffer()
{
    if (activeAnimationCount > 0)
    {
        if (worldMatrixBuffer == prevWorldMatrixBuffer)
        {
            prevWorldMatrixBuffer = Buffer::CreateStructured(sizeof(Vector4), worldMatrixBuffer->GetElementCount(), ResourceBind::ShaderResource, CpuAccess::None, nullptr, false);
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

void AnimationController::BindBuffers()
{
    //TODO
}