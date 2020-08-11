#pragma once

#include "Core/Graph.h"
#include "Render/.Package.h"
#include "RenderCore/RenderContext.h"

class Scene;
class RenderPass;
class RenderGraphCompiler;
class RenderGraphExecutor;
class RenderGraphResourceCache;

class RenderGraph
{
public:
    struct ResourceProperties
    {
        int32 width;
        int32 height;
        ResourceFormat format = ResourceFormat::Unknown;
    };

    struct ExecuteContext
    {
        RenderContext *renderContext;
        ResourceProperties defaultResourceProps;
        RenderGraphResourceCache *resourceCache;
    };

    struct CompileContext
    {
        RenderContext *renderContext;
        ResourceProperties defaultResourceProps;
        const HashMap<String, SPtr<Resource>> &externalResources;
    };

    RenderGraph();

    void SetScene(const SPtr<Scene> &scene);
    int32 AddPass(const String &name, const SPtr<RenderPass> &pass);
    bool RemovePass(const String &name);
    int32 GetPassIndex(const String &name) const;
    SPtr<RenderPass> GetPass(const String &name) const;
    int32 AddEdge(const String &src, const String &dst);
    bool RemoveEdge(const String &src, const String &dst);
    bool RemoveEdge(int32 edgeID);
    int32 GetEdgeIndex(const String &src, const String &dst) const;

    void SetInput(const String &name, const SPtr<Resource> &resource);
    bool IsOutput(const String &name) const;
    SPtr<Resource> GetOutput(const String &name) const;
    void MarkOutput(const String &name);
    void UnmarkOutput(const String &name);
    bool Compile(RenderContext *ctx);
    void Execute(RenderContext *ctx);
    void OnResize(const FrameBuffer *targetFbo);

    bool ContainsPass(const String &name) const
    {
        return GetPassIndex(name) != -1;
    }

    const SPtr<Scene> &GetScene() const
    {
        return scene;
    }

    int32 GetOutputCount() const
    {
        return outputs.Count();
    }

    static SPtr<RenderGraph> Create();

private:
    struct Node
    {
        String name;
        SPtr<RenderPass> pass;
    };

    struct NodePort
    {
        int32 nodeID = -1;
        String fieldName;

        bool operator==(const NodePort &other) const = default;
    };

    struct Edge
    {
        NodePort src;
        NodePort dst;

        int32 From() const
        {
            return src.nodeID;
        }

        int32 To() const
        {
            return dst.nodeID;
        }
    };

    NodePort ParseNodePort(const String &fullname) const;
    int32 GetOutputIndex(const NodePort &port) const;
    void UnmarkOutputs(int32 nodeID);
    bool CheckNodePortField(const NodePort &port, bool input) const;

private:
    friend class RenderGraphCompiler;

    SPtr<Scene> scene;
    Graph<Node, Edge, false> graph;
    HashMap<String, int32> passNameToIndex;
    Array<NodePort> outputs;

    ResourceProperties defaultResourceProps;
    HashMap<String, SPtr<Resource>> externalResources;
    SPtr<RenderGraphExecutor> executor;
    bool recompile = false;
};