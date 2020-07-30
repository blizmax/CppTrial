#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"
#include "Core/Array.h"
#include "Core/HashMap.h"

namespace GraphInternal
{

class Node
{
public:
};

class Edge
{
public:
    Edge() = default;

    Edge(int32 from, int32 to)
        : from(from), to(to)
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

private:
    int32 from = -1;
    int32 to = -1;
};

}

template <template Node = GraphInternal::Node,
          template Edge = GraphInternal::Edge,
          template <typename T> class Alloc = Allocator>
class DirectedGraph
{
public:
    DirectedGraph() = default;

    int32 AddNode()
    {
        nodes.Put(currentNodeID, Node());
        return currentNodeID++;
    }

    bool RemoveNode(int32 nodeID)
    {
        //TODO
        return true;
    }

    // bool ContainsEdge(const Edge &edge) const
    // {
    //     for (auto & e : edges)
    //     {
    //         if (edge.From() == e.From() && edge.To() == e.To())
    //         {

    //         }
    //     }
    // }

    int32 AddEdge(int32 from, int32 to)
    {
        if (!nodes.Contains(from))
        {
            return -1;
        }
        if (!nodes.Contains(to))
        {
            return -1;
        }

        //TODO
    }

    // int32 AddEdge(Edge edge)
    // {

    // }

private:
    HashMap<int32, Node> nodes;
    HashMap<int32, Edge> edges;

    int32 currentNodeID = 0;
    int32 currentEdgeID = 0;
};