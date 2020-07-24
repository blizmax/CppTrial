#pragma once

#include "Render/Animation.h"
#include "Math/Matrix4.h"
#include "Core/HashMap.h"

class Scene;

class AnimationController
{
public:
    AnimationController(Scene *scene);

    void AddAnimation(int32 meshID, const SPtr<Animation> &anim);
    void ToggleAnimations(bool animate);
    bool Animate(RenderContext *ctx, Animation::AnimTimeType currentTime);
    int32 GetAnimationCount(int32 meshID) const;
    SPtr<Animation> GetAnimation(int32 meshID, int32 animID) const;
    bool SetActiveAnimation(int32 meshID, int32 animID);
    int32 GetActiveAnimation(int32 meshID) const;

    const Array<Matrix4> &GetGlobalMatrices() const
    {
        return globalMatrices;
    }

    static UPtr<AnimationController> Create(Scene *scene);

private:
    void CreateSkinningPass();
    void AllocPrevWorldMatrixBuffer();
    void InitLocalMatrices();
    void UpdateMatrices();
    void BindBuffers();

private:
    friend class SceneBuilder;

    struct MeshAnimation
    {
        Array<SPtr<Animation>> animations;
        int32 activeAnimation = -1;
    };

    Scene *scene;
    int32 activeAnimationCount = 0;
    Animation::AnimTimeType lastAnimationTime = 0.0;
    bool animationChanged = true;

    Array<Matrix4> localMatrices;
    Array<Matrix4> globalMatrices;
    Array<Matrix4> invTransposeGlobalMatrices;
    Array<bool> matricesChanged;

    HashMap<int32, MeshAnimation> meshes;

    SPtr<Buffer> worldMatrixBuffer;
    SPtr<Buffer> prevWorldMatrixBuffer;
    SPtr<Buffer> invTransposeWorldMatrixBuffer;
};