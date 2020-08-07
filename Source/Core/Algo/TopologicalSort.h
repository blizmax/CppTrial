#pragma once

#include "Core/Graph.h"
#include "Core/Stack.h"

namespace AlgoInternal
{

template <typename GraphType>
CT_INLINE void TopologicalSortPrivate(const GraphType &graph, int32 nodeID, HashSet<int32> &visited, Stack<int32> &nodes)
{
    visited.Add(nodeID);

    for (int32 e : graph.GetForwardEdges(nodeID))
    {
        int32 n = graph.GetEdge(e).To();
        if (!visited.Contains(n))
        {
            TopologicalSortPrivate(graph, n, visited, nodes);
        }
    }

    nodes.Push(nodeID);
}

}

namespace Algo
{

template <typename GraphType>
CT_INLINE Array<int32> TopologicalSort(const GraphType &graph)
{
    HashSet<int32> visited;
    Stack<int32> nodes;

    for (const auto &[nodeID, node] : graph.GetNodes())
    {
        if (!visited.Contains(nodeID))
        {
            AlgoInternal::TopologicalSortPrivate(graph, nodeID, visited, nodes);
        }
    }

    Array<int32> result;
    result.Reserve(nodes.Count());
    while (!nodes.IsEmpty())
    {
        result.Add(nodes.Top());
        nodes.Pop();
    }
    return result;
}

}