#pragma once

#include "Core/General.h"
#include "Core/Allocator.h"

CT_SCOPE_BEGIN

namespace ListInternal
{
template <typename T>
class Node
{
public:
    Node(const T &val) : value(val), prev(nullptr), next(nullptr)
    {
    }

    T &GetValue()
    {
        return value;
    }

    const T &GetValue() const
    {
        return value;
    }

    void SetNext(Node *node)
    {
        next = node;
    }

    void SetPrev(Node *node)
    {
        prev = node;
    }

    Node *GetPrev()
    {
        return prev;
    }

    const Node *GetPrev() const
    {
        return prev;
    }

    Node *GetNext()
    {
        return next;
    }

    const Node *GetNext() const
    {
        return next;
    }

private:
    Node *prev;
    Node *next;
    T value;
};
} // namespace ListInternal

template <typename Element,
          template <typename T> class Node = ListInternal::Node,
          template <typename T> class Alloc = Allocator>
class List
{
public:
    using NodeType = Node<Element>;

    List() = default;

    List(const List &other)
    {
        for (auto v : other)
        {
            Add(v);
        }
    }

    List(List &&other) noexcept
        : head(other.head), tail(other.tail), size(other.size)
    {
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    List(std::initializer_list<Element> initList)
    {
        for (auto v : initList)
        {
            Add(v);
        }
    }

    List &operator=(const List &other)
    {
        if (this != &other)
        {
            List temp(other);
            Swap(temp);
        }
        return *this;
    }

    List &operator=(List &&other) noexcept
    {
        if (this != &other)
        {
            List temp(std::move(other));
            Swap(temp);
        }
        return *this;
    }

    List &operator=(std::initializer_list<Element> initList)
    {
        List temp(initList);
        Swap(temp);
        return *this;
    }

    ~List()
    {
        Clear();
    }

    SizeType Size() const
    {
        return size;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    NodeType *GetHead() const
    {
        return head;
    }

    NodeType *GetTail() const
    {
        return tail;
    }

    void Swap(List &other)
    {
        if (this != &other)
        {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(size, other.size);
        }
    }

    void Clear()
    {
        NodeType *node;
        while (head)
        {
            node = head;
            head = head->GetNext();
            DeleteNode(node);
        }

        head = tail = nullptr;
        size = 0;
    }

    bool Contains(const Element &value)
    {
        return FindNode(value) != nullptr;
    }

    NodeType *FindNode(const Element &value)
    {
        NodeType *node = head;
        while (node)
        {
            if (node->GetValue() == value)
            {
                break;
            }
            node = node->GetNext();
        }
        return node;
    }

    // since NodeType may only support forward link, always search from head
    NodeType *FindNode(SizeType index)
    {
        if (index >= size)
        {
            return nullptr;
        }

        NodeType *node = head;
        while (index)
        {
            node = node->GetNext();
            --index;
        }
        return node;
    }

    void RemoveNode(NodeType *node)
    {
        if (node == nullptr)
            return;

        if (node == head)
        {
            RemoveFirst();
        }
        else if (node == tail)
        {
            RemoveLast();
        }
        else
        {
            NodeType *next = node->GetNext();
            NodeType *prev = node->GetPrev();
            if (prev)
            {
                prev->SetNext(next);
            }
            if (next)
            {
                next->SetPrev(prev);
            }
            DeleteNode(node);
            --size;
        }
    }

    void RemoveFirst()
    {
        if (size == 0)
            return;
        NodeType *node = head->GetNext();
        if (node)
        {
            node->SetPrev(nullptr);
            DeleteNode(head);
            head = node;
        }
        else
        {
            DeleteNode(head);
            head = tail = nullptr;
        }
        --size;
    }

    void RemoveLast()
    {
        if (size == 0)
            return;
        NodeType *node = tail->GetPrev();
        if (node)
        {
            node->SetNext(nullptr);
            DeleteNode(tail);
            tail = node;
        }
        else
        {
            DeleteNode(tail);
            head = tail = nullptr;
        }
        --size;
    }

    void AddFirst(const Element &value)
    {
        NodeType *node = CreateNode(value);
        if (head)
        {
            node->SetNext(head);
            head->SetPrev(node);
            head = node;
        }
        else
        {
            head = tail = node;
        }
        ++size;
    }

    void AddLast(const Element &value)
    {
        Add(value);
    }

    void Add(const Element &value)
    {
        NodeType *node = CreateNode(value);
        if (tail)
        {
            tail->SetNext(node);
            node->SetPrev(tail);
            tail = node;
        }
        else
        {
            head = tail = node;
        }
        ++size;
    }

    void Insert(NodeType *before, const Element &value)
    {
        if (before == nullptr || before == head)
        {
            AddFirst(value);
        }
        else
        {
            NodeType *node = CreateNode(value);
            if (before->GetPrev())
            {
                node->SetPrev(before->GetPrev());
                before->GetPrev()->SetNext(node);
            }
            node->SetNext(before);
            before->SetPrev(node);
            ++size;
        }
    }

    void Insert(SizeType index, const Element &value)
    {
        Insert(FindNode(index), value);
    }

    //===================== STL STYLE =========================
public:
    friend class Iterator;
    friend class ConstIterator;

    class Iterator
    {
    private:
        NodeType *current;

        void Step()
        {
            if (current)
            {
                current = current->GetNext();
            }
        }

    public:
        Iterator(NodeType *node) : current(node)
        {
        }

        Iterator &operator++()
        {
            Step();
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp(*this);
            Step();
            return temp;
        }

        Element &operator*()
        {
            CT_ASSERT(current);
            return current->GetValue();
        }

        Element *operator->()
        {
            CT_ASSERT(current);
            return &(current->GetValue());
        }

        bool operator==(const Iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    class ConstIterator
    {
    private:
        NodeType *current;

        void Step()
        {
            if (current)
            {
                current = current->GetNext();
            }
        }

    public:
        ConstIterator(NodeType *node) : current(node)
        {
        }

        ConstIterator &operator++()
        {
            Step();
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator temp(*this);
            Step();
            return temp;
        }

        const Element &operator*()
        {
            CT_ASSERT(current);
            return current->GetValue();
        }

        const Element *operator->()
        {
            CT_ASSERT(current);
            return &(current->GetValue());
        }

        bool operator==(const ConstIterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return current != other.current;
        }
    };

    Iterator begin()
    {
        return Iterator(head);
    }

    ConstIterator begin() const
    {
        return ConstIterator(head);
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
    NodeType *CreateNode(const Element &value)
    {
        NodeType *node = NodeAlloc::Allocate(1);
        NodeAlloc::Construct(node, value);
        return node;
    }

    void DeleteNode(NodeType *node)
    {
        NodeAlloc::Destroy(node, 1);
        NodeAlloc::Deallocate(node, 1);
    }

private:
    using NodeAlloc = Alloc<NodeType>;

    NodeType *head = nullptr;
    NodeType *tail = nullptr;
    SizeType size = 0;
};

CT_SCOPE_END