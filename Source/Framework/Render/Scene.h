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
    ProgramDefines GetSceneDefines() const;
    SceneUpdateFlags Update(RenderContext *ctx, float currentTime);
    void Render(RenderContext *ctx, GraphicsState *state, GraphicsVars *vars, SceneRenderFlags flags = SceneRender::None);
    void BindSamplerToMaterials(const SPtr<Sampler> &sampler);
    void ResetCamera(bool resetDepthRange = true);
    void AddViewpoint();
    void AddViewpoint(const Vector3 &position, const Vector3 &target, const Vector3 &up);
    void RemoveViewpoint();
    void SelectViewpoint(int32 index);

    const Array<Viewpoint> &GetViewpoints() const
    {
        return viewpoints;
    }

    int32 GetViewpointCount() const
    {
        return viewpoints.Count();
    }

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
        CT_CHECK(controller->GetCamera() == camera);
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
    bool UpdateCamera(bool force = false);
    bool UpdateLights(bool force = false);
    bool UpdateMaterials(bool force = false);
    void UpdateGeometryStats();

    void CreateDrawList();
    void Finalize();

private:
    friend class SceneBuilder;
    friend class AnimationController;

    struct DrawArgs
    {
        SPtr<Buffer> buffer;
        int32 count = 0;
    } clockwiseDrawArgs, counterClockwiseDrawArgs;

    SPtr<Camera> camera;
    SPtr<CameraController> cameraController;
    Array<Viewpoint> viewpoints;
    int32 currentViewpointIndex = 0;

    UPtr<AnimationController> animationController;
    SPtr<RasterizationState> frontClockwiseRS;
    SPtr<RasterizationState> frontCounterClockwiseRS;

    Array<SPtr<Material>> materials;
    Array<SPtr<Light>> lights;

    Array<MeshDesc> meshDesces;
    Array<MeshInstanceData> meshInstanceDatas;
    Array<SceneNode> nodes;
    //Array<Array<int32>> meshGroups;

    Array<AABox> meshBBs;
    AABox sceneBB;
    Array<bool> meshHasDynamicDatas;

    SPtr<VertexArray> vao;
    SPtr<Buffer> meshesBuffer;
    SPtr<Buffer> meshInstancesBuffer;
    SPtr<Buffer> materialsBuffer;
    SPtr<Buffer> lightsBuffer;
    SPtr<ParameterBlock> sceneBlock;

    SceneUpdateFlags updateFlags = SceneUpdate::All;
};