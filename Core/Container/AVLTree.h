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
    Node *left = nullptr;
    Node *right = nullptr;
    int32 leftHeight = 0;
    int32 rightHeight = 0;

    Node(const Element &element) : element(element)
    {
    }

    Node(Element &&element) : element(std::move(element))
    {
    }

    int32 Height() const
    {
        return (leftHeight >= rightHeight ? leftHeight : rightHeight) + 1;
    }
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

    NodeType *CreateNode(const Element &value)
    {
        NodeType *node = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(node, value);
        return node;
    }

    NodeType *CreateNode(Element &&value)
    {
        NodeType *node = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(node, std::move(value));
        return node;
    }

    void DeleteNode(NodeType *node)
    {
        NodeAlloc::Destroy(node, 1);
        NodeAlloc::Deallocate(node, 1);
    }

    NodeType *FindPrivate(const Key &key, NodeType *node) const
    {
        if (!node)
        {
            return nullptr;
        }

        auto val = Compare(KeyTraits::GetKey(node->element), key);
        if (val == 0)
        {
            return node;
        }
        else if (val < 0)
        {
            return FindPrivate(key, node->right);
        }
        else
        {
            return FindPrivate(key, node->left);
        }
    }

    NodeType *InsertPrivate(const Key &key, const Element &value, NodeType *node, bool *updated)
    {
        if (!node)
        {
            return CreateNode(value);
        }

        NodeType *ret = node;
        auto val = Compare(KeyTraits::GetKey(node->element), key);
        if (val == 0)
        {
            if (updated != nullptr)
            {
                node->element = value;
                *updated = true;
            }
        }
        else if (val < 0)
        {
            node->right = InsertPrivate(key, value, node->right, updated);
            node->rightHeight = node->right->Height();
        }
        else
        {
            node->left = InsertPrivate(key, value, node->left, updated);
            node->leftHeight = node->left->Height();
        }

        return Balance(ret);
    }

    NodeType *Balance(NodeType *node)
    {
        if (!node)
        {
            return nullptr;
        }

        NodeType *ret = node;
        auto val = node->leftHeight - node->rightHeight;
        if (val > 1)
        {
            if (node->left->leftHeight >= node->left->rightHeight)
            {
                ret = RotateLeft(node);
            }
            else
            {
                node->left = RotateRight(node->left);
                ret = RotateLeft(node);
            }
        }
        else if (val < -1)
        {
            if (node->right->rightHeight >= node->right->leftHeight)
            {
                ret = RotateRight(node);
            }
            else
            {
                node->right = RotateLeft(node->right);
                ret = RotateRight(node);
            }
        }

        return ret;
    }

    NodeType *RotateLeft(NodeType *node)
    {
        NodeType *left = node->left;
        node->left = left->right;
        node->leftHeight = left->rightHeight;
        left->right = node;
        left->rightHeight = node->Height();

        return left;
    }

    NodeType *RotateRight(NodeType *node)
    {
        NodeType *right = node->right;
        node->right = right->left;
        node->rightHeight = right->leftHeight;
        right->left = node;
        right->leftHeight = node->Height();

        return right;
    }

private:
    using NodeAlloc = Alloc<NodeType>;

    NodeType *root = nullptr;
    SizeType size = 0;
};