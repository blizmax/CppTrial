#pragma once

#include "Core/Graph.h"
#include "Core/Queue.h"
#include "Core/Stack.h"

namespace AlgoInternal
{

template <typename ContainerType, typename GraphType>
class TGraphSearch
{
public:
    TGraphSearch(const GraphType &graph, int32 source, bool forward = true)
        : graph(graph), forward(forward)
    {
        Reset(source);
    }

    bool Reset(int32 source)
    {
        if (graph.ContainsNode(source))
        {
            visited.Clear();
            nodes.Clear();
            nodes.Push(source);

            return true;
        }
        return false;
    }

    int32 Traverse()
    {
        if (nodes.IsEmpty())
            return -1;

        int32 curNode = nodes.Peek();

        while (visited.Contains(curNode))
        {
            nodes.Pop();
            if (nodes.IsEmpty())
                return -1;

            curNode = nodes.Peek();
        }

        visited.Add(curNode);
        nodes.Pop();

        auto &edges = forward ? graph.GetForwardEdges(curNode) : graph.GetBackwardEdges(curNode);
        for (auto &edgeID : edges)
        {
            auto &edge = graph.GetEdge(edgeID);
            nodes.Push(forward ? edge.To() : edge.From());
        }

        return curNode;
    }

    int32 operator()()
    {
        return Traverse();
    }

private:
    const GraphType &graph;
    ContainerType nodes;
    HashSet<int32> visited;
    int32 source;
    bool forward;
};

template <typename T>
using DFS = TGraphSearch<Stack<int32>, T>;

template <typename T>
using BFS = TGraphSearch<Queue<int32>, T>;

}

namespace Algo
{

template <typename GraphType, typename Visitor>
CT_INLINE bool DepthFirstSearch(const GraphType &graph, int32 srcNode, int32 dstNode, Visitor visitor, bool forward = true)
{
    auto search = AlgoInternal::DFS<GraphType>(graph, srcNode, forward);
    while (true)
    {
        int32 n = search();
        if (n == -1)
            break;

        visitor(n);

        if (n == dstNode)
            return true;
    }
    return false;
}

template <typename GraphType, typename Visitor>
CT_INLINE void DepthFirstSearch(const GraphType &graph, int32 srcNode, Visitor visitor, bool forward = true)
{
    DepthFirstSearch(graph, srcNode, -1, visitor, forward);
}

template <typename GraphType>
CT_INLINE bool DepthFirstSearch(const GraphType &graph, int32 srcNode, int32 dstNode, bool forward = true)
{
    auto search = AlgoInternal::DFS<GraphType>(graph, srcNode, forward);
    while (true)
    {
        int32 n = search();
        if (n == -1)
            break;

        if (n == dstNode)
            return true;
    }
    return false;
}

template <typename GraphType, typename Visitor>
CT_INLINE bool BreadthFirstSearch(const GraphType &graph, int32 srcNode, int32 dstNode, Visitor visitor, bool forward = true)
{
    auto search = AlgoInternal::BFS<GraphType>(graph, srcNode, forward);
    while (true)
    {
        int32 n = search();
        if (n == -1)
            break;

        visitor(n);

        if (n == dstNode)
            return true;
    }
    return false;
}

template <typename GraphType, typename Visitor>
CT_INLINE void BreadthFirstSearch(const GraphType &graph, int32 srcNode, Visitor visitor, bool forward = true)
{
    BreadthFirstSearch(graph, srcNode, -1, visitor, forward);
}

template <typename GraphType>
CT_INLINE bool BreadthFirstSearch(const GraphType &graph, int32 srcNode, int32 dstNode, bool forward = true)
{
    auto search = AlgoInternal::BFS<GraphType>(graph, srcNode, forward);
    while (true)
    {
        int32 n = search();
        if (n == -1)
            break;

        if (n == dstNode)
            return true;
    }
    return false;
}


// FIXME Should add functions to Graph class?
template <typename GraphType>
CT_INLINE bool HasPath(const GraphType &graph, int32 srcNode, int32 dstNode)
{
    return DepthFirstSearch(graph, srcNode, dstNode);
}

template <typename GraphType>
CT_INLINE bool HasCycle(const GraphType &graph, int32 srcNode)
{
    return DepthFirstSearch(graph, srcNode, srcNode);
}

}