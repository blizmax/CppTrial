#pragma once

#include "Core/HashMap.h"
#include "RenderCore/.Package.h"

template <typename NodeType, typename EdgeType = void *>
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
        currentID = nextFreeID++;
        nodes.Put(currentID, Node());
    }

    const NodeType &GetCurrentNodeData() const
    {
        return nodes[currentID].data;
    }

    void SetCurrentNodeData(const NodeType &value)
    {
        nodes[currentID].data = value;
    }

    int32 GetRecentCount() const
    {
        return recentMap.Count();
    }

    void MarkAsRecent()
    {
        // Never remove root node.
        CT_CHECK(currentID != 0);

        recentMap.Put(currentID, ++counterValue);

        if (leastRecentID == -1)
        {
            leastRecentID = currentID;
        }
        else
        {
            if (leastRecentID == currentID)
                leastRecentID = FindLeastRecentID();

            if (counterValue - recentMap[leastRecentID] > REMOVE_LEAST_RECENT_COUNTER_SPAN)
            {
                RemoveNode(leastRecentID);

                if (++gcValue >= GC_COUNTER_SPAN)
                {
                    gcValue = 0;
                    GCNodes();
                }
            }
        }
    }

    bool Contains(const EdgeType &e) const
    {
        int32 *ptr = nodes[currentID].edges.TryGet(e);
        return (ptr && *ptr >= 0);
    }

    bool Walk(const EdgeType &e)
    {
        int32 *ptr = nodes[currentID].edges.TryGet(e);
        if (ptr && *ptr >= 0)
        {
            currentID = *ptr;
            return true;
        }
        else
        {
            int32 newID = nextFreeID++;
            nodes[currentID].edges.Put(e, newID);
            nodes.Put(newID, Node());
            currentID = newID;
            return false;
        }
    }

    template <typename Predicate>
    Array<NodeType> FilterByPredicate(const Predicate &pred) const
    {
        Array<NodeType> result;
        for (auto &[k, v] : nodes)
        {
            if (pred(v.data))
                result.Add(v);
        }
        return result;
    }

    template <typename Predicate>
    bool MatchesByPredicate(const Predicate &pred)
    {
        int32 found = -1;
        for (auto &[k, v] : nodes)
        {
            if (k != currentID && pred(v.data))
            {
                found = k;
                break;
            }
        }

        if (found >= 0)
        {
            for (auto &[k, v] : nodes)
            {
                for (auto &[e, i] : v.edges)
                {
                    if (i == currentID)
                        i = found;
                }
            }
            currentID = found;
            return true;
        }
        return false;
    }

private:
    int32 FindLeastRecentID() const
    {
        uint64 min = counterValue + 1;
        int32 id = -1;
        for (auto &[k, v] : recentMap)
        {
            if (v < min)
            {
                min = v;
                id = k;
            }
        }
        return id;
    }

    void RemoveNode(int32 id)
    {
        nodes.Remove(id);

        for (auto &[k, v] : nodes)
        {
            for (auto &[e, i] : v.edges)
            {
                if (i == id)
                    i = -1;
            }
        }

        if (recentMap.Remove(id))
        {
            leastRecentID = FindLeastRecentID();

            CT_LOG(Debug, CT_TEXT("Remove pso done, remove node id{0}, current pso count{1}, node count{2}."), id, GetRecentCount(), nodes.Count());
        }
    }

    int32 GetBadNodeID()
    {
        for (auto &[k, v] : nodes)
        {
            bool bad = true;
            if (!recentMap.Contains(k))
            {
                for (auto &[e, i] : v.edges)
                {
                    if (i >= 0)
                    {
                        bad = false;
                        break;
                    }
                }
                if (bad)
                    return k;
            }
        }
        return -1;
    }

    void GCNodes()
    {
        while (true)
        {
            int32 badID = GetBadNodeID();
            if (badID == -1)
                break;
            RemoveNode(badID);
        }

        CT_LOG(Debug, CT_TEXT("GC state graph done, current pso count{0}, node count{1}."), GetRecentCount(), nodes.Count());
    }

private:
    static constexpr int32 REMOVE_LEAST_RECENT_COUNTER_SPAN = 1000;
    static constexpr int32 GC_COUNTER_SPAN = 10;

    HashMap<int32, Node> nodes;
    HashMap<int32, uint64> recentMap;
    int32 currentID = 0;
    int32 nextFreeID = 0;
    int32 leastRecentID = -1;
    uint32 gcValue = 0;
    uint64 counterValue = 0;
};