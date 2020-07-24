#pragma once

#include "Math/AABox.h"
#include "Render/AnimationController.h"
#include "Render/CameraController.h"
#include "Render/Light.h"
#include "Render/Material.h"
#include "RenderCore/RenderAPI.h"

class Scene
{
public:
    void Update(RenderContext *ctx, float currentTime);
    void Render(RenderContext *ctx, GraphicsState *state, GraphicsVars *vars);
    void BindSamplerToMaterials(const SPtr<Sampler> &sampler);
    void ResetCamera(bool resetDepthRange = true);
    ProgramDefines GetSceneDefines() const;

    void SetCamera(const SPtr<Camera> &cam)
    {
        camera = cam;
    }

    const SPtr<Camera> &GetCamera() const
    {
        return camera;
    }

    void SetCameraController(const SPtr<CameraController> &controller)
    {
        cameraController = controller;
    }

    const SPtr<CameraController> &GetCameraController() const
    {
        return cameraController;
    }

    const AABox &GetSceneBounds() const
    {
        return sceneBB;
    }

    const AABox &GetMeshBounds(int32 meshID) const
    {
        return meshBBs[meshID];
    }

    int32 GetMeshCount() const
    {
        return meshDesces.Count();
    }

    const MeshDesc &GetMesh(int32 meshID) const
    {
        return meshDesces[meshID];
    }

    int32 GetMeshInstanceCount() const
    {
        return meshInstanceDatas.Count();
    }

    const MeshInstanceData &GetMeshInstance(int32 instanceID) const
    {
        return meshInstanceDatas[instanceID];
    }

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
    void UpdateMeshInstanceFlags();
    void UpdateBounds();
    void UpdateCamera();
    void UpdateLights();
    void UpdateMaterials();

    void CreateDrawList();
    void Finalize();

private:
    friend class SceneBuilder;
    friend class AnimationController;

    SPtr<Camera> camera;
    SPtr<CameraController> cameraController;

    UPtr<AnimationController> animationController;

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