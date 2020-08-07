#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"
#include "Core/Array.h"
#include "Core/HashMap.h"
#include "Core/HashSet.h"

namespace GraphInternal
{

class Node
{
public:
    Node() = default;
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

template <typename Node = GraphInternal::Node,
          typename Edge = GraphInternal::Edge,
          bool uniqueEdge = true>
class Graph
{
public:
    Graph() = default;

    bool ContainsNode(int32 nodeID) const
    {
        return nodes.Contains(nodeID);
    }

    bool ContainsEdge(int32 edgeID) const
    {
        return edges.Contains(edgeID);
    }

    bool ContainsEdge(int32 from, int32 to) const
    {
        if (!forwardMap.Contains(from))
        {
            return false;
        }

        for (int32 e : forwardMap[from])
        {
            if (edges[e].To() == to)
            {
                return true;
            }
        }

        return false;
    }

    int32 GetNodeCount() const
    {
        return nodes.Count();
    }

    int32 GetEdgeCount() const
    {
        return edges.Count();
    }

    void Clear()
    {
        nodes.Clear();
        edges.Clear();
        forwardMap.Clear();
        backwardMap.Clear();
    }

    int32 AddNode()
    {
        nodes.Put(currentNodeID, Node());
        return currentNodeID++;
    }

    int32 AddNode(Node node)
    {
        nodes.Put(currentNodeID, std::move(node));
        return currentNodeID++;
    }

    bool RemoveNode(int32 nodeID)
    {
        if (!nodes.Contains(nodeID))
        {
            return false;
        }

        HashSet<int32> edgeIDs;
        RemoveNodeUnchecked(nodeID, edgeIDs);
        return true;
    }

    bool RemoveNode(int32 nodeID, HashSet<int32> &edgeIDs)
    {
        if (!nodes.Contains(nodeID))
        {
            return false;
        }

        RemoveNodeUnchecked(nodeID, edgeIDs);
        return true;
    }

    int32 AddEdge(Edge edge)
    {
        const int32 from = edge.From();
        const int32 to = edge.To();

        if (!PreAddEdge(from, to))
        {
            return -1;
        }

        edges.Put(currentEdgeID, std::move(edge));
        forwardMap[from].Add(currentEdgeID);
        backwardMap[to].Add(currentEdgeID);

        return currentEdgeID++;
    }

    bool RemoveEdge(int32 edgeID)
    {
        if (!edges.Contains(edgeID))
        {
            return false;
        }

        RemoveEdgeUnchecked(edgeID);
        return true;
    }

    Node &GetNode(int32 nodeID)
    {
        return nodes[nodeID];
    }

    const Node &GetNode(int32 nodeID) const
    {
        return nodes[nodeID];
    }

    const HashMap<int32, Node> &GetNodes() const
    {
        return nodes;
    }

    Edge &GetEdge(int32 edgeID)
    {
        return edges[edgeID];
    }

    const Edge &GetEdge(int32 edgeID) const
    {
        return edges[edgeID];
    }

    const HashMap<int32, Edge> &GetEdges() const
    {
        return edges;
    }

    const Array<int32> &GetForwardEdges(int32 nodeID) const
    {
        if (!forwardMap.Contains(nodeID))
        {
            forwardMap.Put(nodeID, {});
        }
        return forwardMap[nodeID];
    }

    const Array<int32> &GetBackwardEdges(int32 nodeID) const
    {
        if (!backwardMap.Contains(nodeID))
        {
            backwardMap.Put(nodeID, {});
        }
        return backwardMap[nodeID];
    }

private:
    void RemoveNodeUnchecked(int32 nodeID, HashSet<int32> &edgeIDs)
    {
        if (forwardMap.Contains(nodeID))
        {
            for (int32 e : forwardMap[nodeID])
            {
                edgeIDs.Add(e);
                edges.Remove(e);
            }
            forwardMap.Remove(nodeID);
        }
        if (backwardMap.Contains(nodeID))
        {
            for (int32 e : backwardMap[nodeID])
            {
                edgeIDs.Add(e);
                edges.Remove(e);
            }
            backwardMap.Remove(nodeID);
        }

        nodes.Remove(nodeID);
    }

    bool PreAddEdge(int32 from, int32 to)
    {
        CT_CHECK(from >= 0);
        CT_CHECK(to >= 0);

        if (!nodes.Contains(from))
        {
            return false;
        }
        if (!nodes.Contains(to))
        {
            return false;
        }

        if (!forwardMap.Contains(from))
        {
            forwardMap.Put(from, {});
        }
        if (!backwardMap.Contains(to))
        {
            backwardMap.Put(to, {});
        }

        if (uniqueEdge)
        {
            for (int32 e : forwardMap[from])
            {
                if (edges[e].To() == to)
                {
                    return false;
                }
            }
            // No need to check backward
        }

        // TODO Acyclic check
        // if (acyclic)
        // {
        //     if (forwardMap.Contains(to))
        //     {
        //         for (int32 e : forwardMap[to])
        //         {
        //             if (edges[e].To() == from)
        //             {
        //                 return false;
        //             }
        //         }
        //     }
        // }

        return true;
    }

    void RemoveEdgeUnchecked(int32 edgeID)
    {
        const auto &e = edges[edgeID];
        const int32 from = e.From();
        const int32 to = e.To();

        forwardMap[from].RemoveValue(edgeID);
        backwardMap[to].RemoveValue(edgeID);
        edges.Remove(edgeID);
    }

private:
    HashMap<int32, Node> nodes;
    HashMap<int32, Edge> edges;
    mutable HashMap<int32, Array<int32>> forwardMap;  // from -> [to0, to1, to2...]
    mutable HashMap<int32, Array<int32>> backwardMap; // to -> [from0, from1, from2...]

    int32 currentNodeID = 0;
    int32 currentEdgeID = 0;
};