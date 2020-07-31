#pragma once

#include "Render/Scene.h"
#include "Render/RenderPass.h"
#include "Core/Graph.h"

class RenderGraph
{
public:
    RenderGraph();

    void SetScene(const SPtr<Scene> &scene);
    int32 AddPass(const String &name, const SPtr<RenderPass> &pass);
    bool RemovePass(const String &name);
    int32 GetPassIndex(const String &name) const;
    SPtr<RenderPass> GetPass(const String &name) const;
    int32 AddEdge(const String &from, const String &to);
    bool RemoveEdge(const String &from, const String &to);
    bool RemoveEdge(int32 edgeID);

    void SetInput(const String &name, const SPtr<Resource> &resource);
    SPtr<Resource> GetOutput(const String &name);

    bool Compile();

    bool ContainsPass(const String &name) const
    {
        return GetPassIndex(name) != -1;
    }

    const SPtr<Scene> &GetScene() const
    {
        return scene;
    }


    static SPtr<RenderGraph> Create();

private:
    struct Node
    {
        String name;
        SPtr<RenderPass> pass;
    };

    struct Edge
    {
        int32 from;
        int32 to;

        Edge(int32 from, int32 to) : from(from), to(to)
        {
        }

        int32 From() const
        {
            return from;
        }

        int32 To() const
        {
            return to;
        }
    };

    SPtr<Scene> scene;
    Graph<Node, Edge> graph;
    HashMap<String, int32> passNameToIndex;

    bool recompile = false;
};