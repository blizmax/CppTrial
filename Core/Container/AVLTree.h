#pragma once

#include "Core/Container/.Package.h"
#include "Core/Allocator.h"
#include "Core/Stack.h"

namespace AVLTreeInternal
{
template <typename Element>
class Node
{
public:
    Node *left = nullptr;
    Node *right = nullptr;
    int32 leftHeight = 0;
    int32 rightHeight = 0;
    Element element;

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

public:
    AVLTree() = default;

    AVLTree(const AVLTree &other) : size(other.size)
    {
        root = CopyFrom(other.root);
    }

    AVLTree(AVLTree &&other) noexcept : size(other.size), root(other.root)
    {
        other.size = 0;
        other.root = nullptr;
    }

    AVLTree &operator=(const AVLTree &other)
    {
        if (this != &other)
        {
            AVLTree temp(other);
            Swap(temp);
        }
        return *this;
    }

    AVLTree &operator=(AVLTree &&other) noexcept
    {
        if (this != &other)
        {
            ClearPrivate(root);

            size = other.size;
            root = other.root;
            other.size = 0;
            other.root = nullptr;
        }
        return *this;
    }

    ~AVLTree()
    {
        ClearPrivate(root);
    }

    SizeType Size() const
    {
        return size;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    void Swap(AVLTree &other)
    {
        if (this != &other)
        {
            std::swap(size, other.size);
            std::swap(root, other.root);
        }
    }

    void Clear()
    {
        ClearPrivate(root);
        root = nullptr;
        size = 0;
    }

    NodeType *Find(const Element &value) const
    {
        const Key &key = KeyTraits::GetKey(value);
        return FindPrivate(key, root);
    }

    NodeType *FindByKey(const Key &key) const
    {
        return FindPrivate(key, root);
    }

    bool Contains(const Element &value) const
    {
        return Find(value) != nullptr;
    }

    bool ContainsKey(const Key &key) const
    {
        return FindByKey(key) != nullptr;
    }

    void Put(const Element &value)
    {
        const Key &key = KeyTraits::GetKey(value);

        root = InsertPrivate(key, value, root, nullptr);
    }

    void Put(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);

        root = InsertPrivate(key, std::move(value), root, nullptr);
    }

    bool Add(const Element &value)
    {
        const Key &key = KeyTraits::GetKey(value);

        bool existed = false;
        root = InsertPrivate(key, value, root, &existed);
        return !existed;
    }

    bool Add(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);

        bool existed = false;
        root = InsertPrivate(key, std::move(value), root, &existed);
        return !existed;
    }

    Element &Get(const Element &value)
    {
        NodeType *node = Find(value);
        CheckRange(node);
        return node->element;
    }

    const Element &Get(const Element &value) const
    {
        NodeType *node = Find(value);
        CheckRange(node);
        return node->element;
    }

    Element &GetByKey(const Key &key)
    {
        NodeType *node = FindByKey(key);
        CheckRange(node);
        return node->element;
    }

    const Element &GetByKey(const Key &key) const
    {
        NodeType *node = FindByKey(key);
        CheckRange(node);
        return node->element;
    }

    bool Remove(const Element &value)
    {
        return RemoveByKey(KeyTraits::GetKey(value));
    }

    bool RemoveByKey(const Key &key)
    {
        bool existed = true;
        root = RemovePrivate(key, root, &existed);
        return existed;
    }

    bool operator==(const AVLTree &other) const
    {
        if (size != other.size)
        {
            return false;
        }

        auto iter0 = begin();
        auto iter1 = other.begin();
        while(iter0 != end() && iter1 != other.end())
        {
            if(*iter0 != *iter1)
            {
                return false;
            }
            ++iter0;
            ++iter1;
        }
        return true;
    }

    bool operator!=(const AVLTree &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    class IteratorBase
    {
    protected:
        Stack<NodeType *> stack;

        void PushLeft(NodeType *root)
        {
            if (!root)
            {
                return;
            }

            NodeType *current = root;
            while (current)
            {
                stack.Push(current);
                current = current->left;
            }
        }

        void Step()
        {
            if (stack.IsEmpty())
            {
                return;
            }
            NodeType *top = stack.Top();
            stack.Pop();
            PushLeft(top->right);
        }

    public:
        bool operator==(const IteratorBase &other) const
        {
            NodeType *ptr0 = stack.IsEmpty() ? nullptr : stack.Top();
            NodeType *ptr1 = other.stack.IsEmpty() ? nullptr : other.stack.Top();
            return ptr0 == ptr1;
        }

        bool operator!=(const IteratorBase &other) const
        {
            return !(*this == other);
        }
    };

    class Iterator : public IteratorBase
    {
    private:
        using IteratorBase::stack;
    public:
        Iterator(NodeType *root)
        {
            this->PushLeft(root);
        }

        Iterator &operator++()
        {
            this->Step();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp(*this);
            this->Step();
            return temp;
        }

        Element &operator*()
        {
            return stack.Top()->element;
        }

        Element *operator->()
        {
            return &(stack.Top()->element);
        }
    };

    class ConstIterator : public IteratorBase
    {
    private:
        using IteratorBase::stack;
    public:
        ConstIterator(NodeType *root)
        {
            PushLeft(root);
        }

        ConstIterator &operator++()
        {
            this->Step();
            return *this;
        }

        ConstIterator operator++(int)
        {
            Iterator temp(*this);
            this->Step();
            return temp;
        }

        const Element &operator*()
        {
            return stack.Top()->element;
        }

        const Element *operator->()
        {
            return &(stack.Top()->element);
        }
    };

    Iterator begin()
    {
        return Iterator(root);
    }

    ConstIterator begin() const
    {
        return ConstIterator(root);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    ConstIterator end() const
    {
        return ConstIterator(nullptr);
    }

private:
    void CheckRange(NodeType *node) const
    {
        CT_ASSERT(node != nullptr);
    }

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

    NodeType *FindMinPrivate(NodeType *node) const
    {
        if (node->left)
        {
            return FindMinPrivate(node->left);
        }
        return node;
    }

    NodeType *FindMaxPrivate(NodeType *node) const
    {
        if (node->right)
        {
            return FindMaxPrivate(node->right);
        }
        return node;
    }

    NodeType *CopyFrom(NodeType *node)
    {
        if (!node)
        {
            return nullptr;
        }

        NodeType *newNode = CreateNode(node->element);
        newNode->left = CopyFrom(node->left);
        newNode->right = CopyFrom(node->right);
        newNode->leftHeight = node->leftHeight;
        newNode->rightHeight = node->rightHeight;
        return newNode;
    }

    void ClearPrivate(NodeType *node)
    {
        if (!node)
        {
            return;
        }

        ClearPrivate(node->left);
        ClearPrivate(node->right);
        DeleteNode(node);
    }

    NodeType *InsertPrivate(const Key &key, const Element &value, NodeType *node, bool *existed)
    {
        if (!node)
        {
            ++size;
            return CreateNode(value);
        }

        NodeType *ret = node;
        auto val = Compare(KeyTraits::GetKey(node->element), key);
        if (val == 0)
        {
            if (existed == nullptr)
            {
                node->element = value;
            }
            else
            {
                *existed = true;
            }
        }
        else if (val < 0)
        {
            node->right = InsertPrivate(key, value, node->right, existed);
            node->rightHeight = node->right->Height();
        }
        else
        {
            node->left = InsertPrivate(key, value, node->left, existed);
            node->leftHeight = node->left->Height();
        }

        return Balance(ret);
    }

    NodeType *InsertPrivate(const Key &key, Element &&value, NodeType *node, bool *existed)
    {
        if (!node)
        {
            ++size;
            return CreateNode(std::move(value));
        }

        NodeType *ret = node;
        auto val = Compare(KeyTraits::GetKey(node->element), key);
        if (val == 0)
        {
            if (existed == nullptr)
            {
                node->element = std::move(value);
            }
            else
            {
                *existed = true;
            }
        }
        else if (val < 0)
        {
            node->right = InsertPrivate(key, std::move(value), node->right, existed);
            node->rightHeight = node->right->Height();
        }
        else
        {
            node->left = InsertPrivate(key, std::move(value), node->left, existed);
            node->leftHeight = node->left->Height();
        }

        return Balance(ret);
    }

    NodeType *RemovePrivate(const Key &key, NodeType *node, bool *existed)
    {
        if (!node)
        {
            *existed = false;
            return nullptr;
        }

        NodeType *ret = node;
        auto val = Compare(KeyTraits::GetKey(node->element), key);
        if (val == 0)
        {
            if (!node->left && !node->right)
            {
                DeleteNode(node);
                ret = nullptr;
                --size;
            }
            else if (node->left && node->right)
            {
                NodeType *max = FindMaxPrivate(node->left);
                NodeType *newNode = CreateNode(max->element);
                newNode->left = RemovePrivate(KeyTraits::GetKey(newNode->element), node->left, existed);
                newNode->right = node->right;
                newNode->leftHeight = newNode->left ? newNode->left->Height() : 0;
                newNode->rightHeight = node->rightHeight;

                DeleteNode(node);
                ret = newNode;
            }
            else
            {
                NodeType *temp = (node->left ? node->left : node->right);

                DeleteNode(node);
                ret = temp;
                --size;
            }
        }
        else if (val < 0)
        {
            node->right = RemovePrivate(key, node->right, existed);
            if (*existed)
            {
                node->rightHeight = node->right ? node->right->Height() : 0;
            }
        }
        else
        {
            node->left = RemovePrivate(key, node->left, existed);
            if (*existed)
            {
                node->leftHeight = node->left ? node->left->Height() : 0;
            }
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