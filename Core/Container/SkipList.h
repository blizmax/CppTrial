#pragma once

#include "Core/Container/.Package.h"
#include "Core/Allocator.h"
#include "Core/Math.h"

namespace SkipListInternal
{
template <typename T>
struct Comparator
{
    int32 operator()(const T &a, const T &b) const
    {
        if(a < b)
            return -1;
        else if(a == b)
            return 0;
        else
            return 1;
    }
};

template <typename Element>
class Node
{
public:
    Element element;
    Node **forward = nullptr;
    uint32 level = 0;

    Node() = default;

    Node(const Element &element) : element(element)
    {
    }
};
} // namespace SkipListInternal

template <typename Element, typename Comparer, typename KeyTraits, template <typename> class Alloc>
class SkipList
{
public:
    using Key = typename KeyTraits::KeyType;
    using NodeType = SkipListInternal::Node<Element>;

    static constexpr uint32 MAX_LEVEL = 16;

public:
    SkipList()
    {
        Init();
    }

    void Clear()
    {
        //TODO
    }

    bool Add(const Element &value)
    {
        const Key &key = KeyTraits::GetKey(value);

        NodeType *cache[MAX_LEVEL];
        NodeType *node = FindPrivate(key, cache);
        if (node != nullptr)
        {
            return false;
        }

        uint32 nodeLevel = static_cast<uint32>(Math::RandInt(0, MAX_LEVEL - 1));
        if(nodeLevel > level)
        {
            nodeLevel = ++level;
            cache[level] = head;
        }
        NodeType *newNode = CreateNode(nodeLevel, value);
        for(uint32 i = level; i > 0; --i)
        {
            node = cache[i];
            newNode->forward[i] = node->forward[i];
            node->forward[i] = newNode; 
        }
        
        ++size;

        return true;
    }

    bool Remove(const Element &value)
    {
        return RemovePrivate(KeyTraits::GetKey(value));
    }

    bool RemoveByKey(const Key &key)
    {
        return RemovePrivate(key);
    }

private:
    void Init()
    {
        head = NodeAlloc::Allocate(1);
        head->level = MAX_LEVEL - 1;
        head->forward = NodePtrAlloc::Allocate(MAX_LEVEL);
        for (uint32 i = 0; i < MAX_LEVEL; ++i)
        {
            head->forward[i] = nullptr;
        }
    }

    NodeType *CreateNode(uint32 level, const Element &value)
    {
        NodeType *node = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(node, value);
        node->level = level;
        node->forward = NodePtrAlloc::Allocate(level + 1);
        return node;
    }

    void DeleteNode(NodeType *node)
    {
        if (node->forward)
        {
            NodePtrAlloc::Deallocate(node->forward);
            node->forward = nullptr;
        }

        NodeAlloc::Destroy(node, 1);
        NodeAlloc::Deallocate(node, 1);
    }

    int32 Compare(const Key &key1, const Key &key2) const
    {
        static Comparer comp;
        return comp(key1, key2);
    }

    NodeType *FindPrivate(const Key &key) const
    {
        NodeType *node = head;
        for (uint32 i = level; i > 0; --i)
        {
            while (node->forward[i] && Compare(node->forward[i]->key, key) < 0)
            {
                node = node->forward[i];
            }
        }

        if (node->forward[0] != nullptr)
        {
            node = node->forward[0];
            if (Compare(node->key, key) == 0)
            {
                return node;
            }
        }

        return nullptr;
    }

    NodeType *FindPrivate(const Key &key, NodeType **cache) const
    {
        NodeType *node = head;
        for (uint32 i = level; i > 0; --i)
        {
            while (node->forward[i] && Compare(node->forward[i]->key, key) < 0)
            {
                node = node->forward[i];
            }
            cache[i] = node;
        }

        if (node->forward[0] != nullptr)
        {
            node = node->forward[0];
            if (Compare(node->key, key) == 0)
            {
                return node;
            }
        }

        return nullptr;
    }

    bool RemovePrivate(const Key& key)
    {
        NodeType *cache[MAX_LEVEL];
        NodeType *node = FindPrivate(key, cache);
        if (node == nullptr)
        {
            return false;
        }

        for(uint32 i = 0; i <= level; ++i)
        {
            if(cache[i]->forward[i] != node)
            {
                break;
            }
            cache[i]->forward[i] = node->forward[i];
        }

        DeleteNode(node);

        while(level > 0 && head->forward[level] == nullptr)
        {
            --level;
        }

        --size;

        return true;
    }

private:
    using NodeAlloc = Alloc<NodeType>;
    using NodePtrAlloc = Alloc<NodeType *>;

    NodeType *head = nullptr;
    uint32 level = 0;
    SizeType size = 0;
};