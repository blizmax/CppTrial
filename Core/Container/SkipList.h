#pragma once

#include "Core/Container/.Package.h"
#include "Core/Allocator.h"
#include "Core/Math.h"

namespace SkipListInternal
{
template <typename Element, template <typename T> class Alloc = Allocator>
class Node
{
public:
    using NodePtrAlloc = Alloc<Node*>;

    Element element;
    Node **forward;
    int32 level = 0;

    Node(int32 level) : level(level)
    {
        Init();
    }

    Node(const Element &element, int32 level) : element(element), level(level)
    {
        Init();
    }

    Node(Element &&element, int32 level) : element(std::move(element)), level(level)
    {
        Init();
    }

    ~Node()
    {
        if (forward)
        {
            NodePtrAlloc::Deallocate(forward);
            forward = nullptr;
        }
    }

private:
    void Init()
    {
        forward = NodePtrAlloc::Allocate(level + 1);
        for (int32 i = 0; i <= level; ++i)
        {
            forward[i] = nullptr;
        }
    }
};
} // namespace SkipListInternal

template <typename Element, typename Comparer, typename KeyTraits, template <typename> class Alloc>
class SkipList
{
public:
    using Key = typename KeyTraits::KeyType;
    using NodeType = SkipListInternal::Node<Element>;

    static constexpr int32 MAX_LEVEL = 16;

public:
    SkipList()
    {
        CreateHead();
    }

    SkipList(const SkipList &other)
    {
        CreateHead();

        NodeType *current = other.head->forward[0];
        while (current)
        {
            Add(current->element);
            current = current->forward[0];
        }
    }

    SkipList(SkipList &&other) noexcept : size(other.size), level(other.level), head(other.head)
    {
        other.size = 0;
        other.level = 0;
        other.head = nullptr;
    }

    SkipList &operator=(const SkipList &other)
    {
        if (this != &other)
        {
            SkipList temp(other);
            Swap(temp);
        }
        return *this;
    }

    SkipList &operator=(SkipList &&other) noexcept
    {
        if (this != &other)
        {
            Clear();

            size = other.size;
            level = other.level;
            head = other.head;
            other.size = 0;
            other.level = 0;
            other.head = nullptr;
        }
        return *this;
    }

    ~SkipList()
    {
        Clear();
        DeleteHead();
    }

    SizeType Size() const
    {
        return size;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    void Swap(SkipList &other)
    {
        if (this != &other)
        {
            std::swap(size, other.size);
            std::swap(level, other.level);
            std::swap(head, other.head);
        }
    }

    void Clear()
    {
        for (int32 i = MAX_LEVEL - 1; i > 0; --i)
        {
            head->forward[i] = nullptr;
        }

        NodeType *current = head->forward[0];
        while (current)
        {
            NodeType *next = current->forward[0];
            DeleteNode(current);
            current = next;
        }

        head->forward[0] = nullptr;
        level = 0;
        size = 0;
    }

    NodeType *Find(const Element &value) const
    {
        const Key &key = KeyTraits::GetKey(value);
        return FindPrivate(key);
    }

    NodeType *FindByKey(const Key &key) const
    {
        return FindPrivate(key);
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

        NodeType *cache[MAX_LEVEL];
        NodeType *node = FindPrivate(key, cache);
        if (node != nullptr)
        {
            node->element = value;
        }
        else
        {
            InsertPrivate(key, value, cache);
        }
    }

    void Put(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);

        NodeType *cache[MAX_LEVEL];
        NodeType *node = FindPrivate(key, cache);
        if (node != nullptr)
        {
            node->element = std::move(value);
        }
        else
        {
            InsertPrivate(key, std::move(value), cache);
        }
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

        InsertPrivate(key, value, cache);

        return true;
    }

    bool Add(Element &&value)
    {
        const Key &key = KeyTraits::GetKey(value);

        NodeType *cache[MAX_LEVEL];
        NodeType *node = FindPrivate(key, cache);
        if (node != nullptr)
        {
            return false;
        }

        InsertPrivate(key, std::move(value), cache);

        return true;
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
        return RemovePrivate(KeyTraits::GetKey(value));
    }

    bool RemoveByKey(const Key &key)
    {
        return RemovePrivate(key);
    }

    bool operator==(const SkipList &other) const
    {
        if (size != other.size)
        {
            return false;
        }

        NodeType *ptr0 = head->forward[0];
        NodeType *ptr1 = other->forward[0];
        while (ptr0 && ptr1)
        {
            if (ptr0->element != ptr1->element)
            {
                return false;
            }
            ptr0 = ptr0->forward[0];
            ptr1 = ptr1->forward[0];
        }
        return true;
    }

    bool operator!=(const SkipList &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    friend class Iterator;
    friend class ConstIterator;

    class Iterator
    {
    private:
        NodeType *node;

    public:
        Iterator(NodeType *node) : node(node)
        {
        }

        Iterator(const Iterator &other) : node(other.node)
        {
        }

        Iterator &operator++()
        {
            node = node->forward[0];
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp(*this);
            node = node->forward[0];
            return temp;
        }

        Element &operator*()
        {
            return node->element;
        }

        Element *operator->()
        {
            return &(node->element);
        }

        bool operator==(const Iterator &other) const
        {
            return node == other.node;
        }

        bool operator!=(const Iterator &other) const
        {
            return node != other.node;
        }
    };

    class ConstIterator
    {
    private:
        NodeType *node;

    public:
        ConstIterator(NodeType *node) : node(node)
        {
        }

        ConstIterator(const ConstIterator &other) : node(other.node)
        {
        }

        ConstIterator &operator++()
        {
            node = node->forward[0];
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator temp(*this);
            node = node->forward[0];
            return temp;
        }

        const Element &operator*()
        {
            return node->element;
        }

        const Element *operator->()
        {
            return &(node->element);
        }

        bool operator==(const Iterator &other) const
        {
            return node == other.node;
        }

        bool operator!=(const Iterator &other) const
        {
            return node != other.node;
        }
    };

    Iterator begin()
    {
        return Iterator(head->forward[0]);
    }

    ConstIterator begin() const
    {
        return ConstIterator(head->forward[0]);
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

    void CreateHead()
    {
        head = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(head, MAX_LEVEL - 1);
    }

    void DeleteHead()
    {
        NodeAlloc::Destroy(head, 1);
        NodeAlloc::Deallocate(head, 1);
    }

    NodeType *CreateNode(int32 level, const Element &value)
    {
        NodeType *node = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(node, value, level);
        return node;
    }

    NodeType *CreateNode(int32 level, Element &&value)
    {
        NodeType *node = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(node, std::move(value), level);
        return node;
    }

    void DeleteNode(NodeType *node)
    {
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
        for (int32 i = level; i >= 0; --i)
        {
            while (node->forward[i] && Compare(KeyTraits::GetKey(node->forward[i]->element), key) < 0)
            {
                node = node->forward[i];
            }
        }

        if (node->forward[0] != nullptr)
        {
            node = node->forward[0];
            if (Compare(KeyTraits::GetKey(node->element), key) == 0)
            {
                return node;
            }
        }

        return nullptr;
    }

    NodeType *FindPrivate(const Key &key, NodeType **cache) const
    {
        NodeType *node = head;
        for (int32 i = level; i >= 0; --i)
        {
            while (node->forward[i] && Compare(KeyTraits::GetKey(node->forward[i]->element), key) < 0)
            {
                node = node->forward[i];
            }
            cache[i] = node;
        }

        if (node->forward[0] != nullptr)
        {
            node = node->forward[0];
            if (Compare(KeyTraits::GetKey(node->element), key) == 0)
            {
                return node;
            }
        }

        return nullptr;
    }

    void InsertPrivate(const Key &key, const Element &value, NodeType **cache)
    {
        int32 nodeLevel = Math::RandInt(0, MAX_LEVEL - 1);
        if (nodeLevel > level)
        {
            nodeLevel = ++level;
            cache[level] = head;
        }
        NodeType *newNode = CreateNode(nodeLevel, value);
        for (int32 i = nodeLevel; i >= 0; --i)
        {
            newNode->forward[i] = cache[i]->forward[i];
            cache[i]->forward[i] = newNode;
        }

        ++size;
    }

    void InsertPrivate(const Key &key, Element &&value, NodeType **cache)
    {
        int32 nodeLevel = Math::RandInt(0, MAX_LEVEL - 1);
        if (nodeLevel > level)
        {
            nodeLevel = ++level;
            cache[level] = head;
        }
        NodeType *newNode = CreateNode(nodeLevel, std::move(value));
        for (int32 i = nodeLevel; i >= 0; --i)
        {
            newNode->forward[i] = cache[i]->forward[i];
            cache[i]->forward[i] = newNode;
        }

        ++size;
    }

    bool RemovePrivate(const Key &key)
    {
        NodeType *cache[MAX_LEVEL];
        NodeType *node = FindPrivate(key, cache);
        if (node == nullptr)
        {
            return false;
        }

        for (int32 i = 0; i <= level; ++i)
        {
            if (cache[i]->forward[i] != node)
            {
                break;
            }
            cache[i]->forward[i] = node->forward[i];
        }

        DeleteNode(node);

        while (level > 0 && head->forward[level] == nullptr)
        {
            --level;
        }

        --size;

        return true;
    }

private:
    using NodeAlloc = Alloc<NodeType>;

    NodeType *head = nullptr;
    int32 level = 0;
    SizeType size = 0;
};