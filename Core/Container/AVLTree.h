#pragma once

#include "Core/Container/.Package.h"
#include "Core/Allocator.h"

namespace AVLTreeInternal
{
template <typename Element>
class Node
{
public:
    Element element;
    Node *left;
    Node *right;
    int32 height;
};
} // namespace AVLTreeInternal

template <typename Element, typename Comparer, typename KeyTraits, template <typename> class Alloc>
class AVLTree
{
public:
    using Key = typename KeyTraits::KeyType;
    using NodeType = AVLTreeInternal::Node<Element>;

private:
    int32 Compare(const Key &key1, const Key &key2) const
    {
        static Comparer comp;
        return comp(key1, key2);
    }

    NodeType *FindPrivate(const Key &key, NodeType *node) const
    {
        if (!node)
        {
            return nullptr;
        }

        auto ret = Compare(KeyTraits::GetKey(node->element), key);
        if (ret == 0)
        {
            return node;
        }
        else if (ret < 0)
        {
            return FindPrivate(key, node->right);
        }
        else
        {
            return FindPrivate(key, node->left);
        }
    }

    void InsertPrivate(const Key &key, const Element &value, NodeType *node)
    {
        //TODO
    }

private:
    using NodeAlloc = Alloc<NodeType>;

    NodeType *root = nullptr;
    SizeType size = 0;
};