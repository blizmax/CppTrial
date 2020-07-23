#pragma once

#include "Render/AnimationController.h"
#include "Render/CameraController.h"
#include "Render/Light.h"
#include "Render/Material.h"
#include "RenderCore/RenderAPI.h"
#include "Math/AABox.h"

class Scene
{
public:
    void BindSamplerToMaterials(const SPtr<Sampler> &sampler);
    void Render(RenderContext *ctx, GraphicsState *state, GraphicsVars *vars);

    int32 GetMaterialCount() const
    {
        return materials.Count();
    }

    const SPtr<Material> &GetMaterial(int32 index) const
    {
        return materials[index];
    }

    const Array<SPtr<Material>> &GetMaterials() const
    {
        return materials;
    }

    int32 GetLightCount() const
    {
        return lights.Count();
    }

    const SPtr<Light> &GetLight(int32 index) const
    {
        return lights[index];
    }

    const Array<SPtr<Light>> &GetLights() const
    {
        return lights;
    }

    static SPtr<Scene> Create();

private:
    void SortMeshes();
    void InitResources();
    void UploadResources();
    void UploadMaterial(int32 matID);
    void UpdateBounds();
    void CreateDrawList();
    void Finalize();

private:
    friend class SceneBuilder;
    friend class AnimationController;

    SPtr<Camera> camera;
    SPtr<CameraController> cameraController;
    SPtr<AnimationController> animationController;

    Array<SPtr<Material>> materials;
    Array<SPtr<Light>> lights;

    Array<MeshDesc> meshDesces;
    Array<MeshInstanceData> meshInstanceDatas;
    Array<Array<int32>> meshGroups;
    Array<SceneNode> nodes;

    Array<AABox> meshBBs;
    AABox sceneBB;
    Array<bool> meshHasDynamicDatas;

    SPtr<VertexArray> vao;
    SPtr<Buffer> meshesBuffer;
    SPtr<Buffer> meshInstancesBuffer;
    SPtr<Buffer> materialsBuffer;
    SPtr<Buffer> lightsBuffer;
    SPtr<ParameterBlock> sceneBlock;
};