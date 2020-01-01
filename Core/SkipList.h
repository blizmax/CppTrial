#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"
#include "Core/Math.h"

namespace SkipListInternal
{
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
        NodeType *node = FindPrivate(key);

        if (node)
        {
            return false;
        }

        NodeType *newNode = CreateNode(value);

        //TODO

        ++size;

        return true;
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

private:
    using NodeAlloc = Alloc<NodeType>;
    using NodePtrAlloc = Alloc<NodeType *>;

    NodeType *head = nullptr;
    uint32 level = 0;
    SizeType size = 0;
};