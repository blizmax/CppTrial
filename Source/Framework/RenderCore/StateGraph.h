#pragma once

#include "RenderCore/.Package.h"
#include "Core/HashMap.h"

template <typename NodeType, typename EdgeType = void*>
class StateGraph
{
public:
    using EdgeMap = HashMap<EdgeType, int32>;
    struct Node
    {
        NodeType data{};
        EdgeMap edges;
    };

    StateGraph()
    {
        nodes.SetCount(1);
    }

    const NodeType &GetCurrentNodeData() const
    {
        return nodes[currentIndex].data;
    }

    void SetCurrentNodeData(const NodeType &value)
    {
        nodes[currentIndex].data = value;
    }

    bool Contains(const EdgeType &e) const
    {
        return nodes[currentIndex].edges.Contains(e);
    }

    bool Walk(const EdgeType &e)
    {
        int32 *ptr = nodes[currentIndex].edges.TryGet(e);
        if(ptr)
        {
            currentIndex = *ptr;
            return true;
        }
        else
        {
            int32 newIndex = nodes.Count();
            nodes[currentIndex].edges.Put(e, newIndex);
            nodes.Add(Node{});
            currentIndex = newIndex;
            return false;
        }
    }

    template <typename Compare>
    bool ScanForMatching(const Compare &comp)
    {
        for(int32 i = 0; i < nodes.Count(); ++i)
        {
            if(i != currentIndex && comp(nodes[i].data))
            {
                for(int32 j = 0; j < nodes.Count(); ++j)
                {
                    for(auto &[k, v] : nodes[j].edges)
                    {
                        if(v == currentIndex)
                            v = i;
                    }
                }
                currentIndex = i;
                return true;
            }
        }
        return false;
    }

private:
    Array<Node> nodes;
    int32 currentIndex = 0;
};