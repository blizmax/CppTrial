#pragma once

#include "Render/Scene.h"
#include "Render/RenderPass.h"
#include "Core/HashMap.h"

class RenderGraph
{
public:
    RenderGraph();

    void SetScene(const SPtr<Scene> &scene);
    int32 AddPass(const String &name, const SPtr<RenderPass> &pass);
    bool RemovePass(const String &name);
    int32 GetPassIndex(const String &name) const;
    const SPtr<RenderPass> &GetPass(const String &name) const;

    static SPtr<RenderGraph> Create();

private:
    struct NodeData
    {
        String name;
        SPtr<RenderPass> pass;
    };

    SPtr<Scene> scene;

    HashMap<String, int32> passNameToIndex;
};