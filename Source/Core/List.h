#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"

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
        : head(other.head), tail(other.tail), count(other.count)
    {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
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

    int32 Count() const
    {
        return count;
    }

    bool IsEmpty() const
    {
        return count == 0;
    }

    NodeType *GetHead() const
    {
        return head;
    }

    NodeType *GetTail() const
    {
        return tail;
    }

    void Swap(List &other) noexcept
    {
        if (this != &other)
        {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(count, other.count);
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
        count = 0;
    }

    bool Contains(const Element &value) const
    {
        return FindNode(value) != nullptr;
    }

    NodeType *FindNode(const Element &value) const
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

    NodeType *FindNodeByIndex(int32 index) const
    {
        if (index >= count || index < 0)
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

    void AddFirst(const Element &value)
    {
        NodeType *node = CreateNode(value);
        AddFirstPrivate(node);
    }

    void AddFirst(Element &&value)
    {
        NodeType *node = CreateNode(std::move(value));
        AddFirstPrivate(node);
    }

    void AddLast(const Element &value)
    {
        NodeType *node = CreateNode(value);
        AddLastPrivate(node);
    }

    void AddLast(Element &&value)
    {
        NodeType *node = CreateNode(std::move(value));
        AddLastPrivate(node);
    }

    void Add(const Element &value)
    {
        NodeType *node = CreateNode(value);
        AddLastPrivate(node);
    }

    void Add(Element &&value)
    {
        NodeType *node = CreateNode(std::move(value));
        AddLastPrivate(node);
    }

    void Insert(NodeType *before, const Element &value)
    {
        NodeType *node = CreateNode(value);
        InsertPrivate(before, node);
    }

    void Insert(NodeType *before, Element &&value)
    {
        NodeType *node = CreateNode(std::move(value));
        InsertPrivate(before, node);
    }

    void Insert(int32 index, const Element &value)
    {
        Insert(FindNodeByIndex(index), value);
    }

    void Insert(int32 index, Element &&value)
    {
        Insert(FindNodeByIndex(index), std::move(value));
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
            --count;
        }
    }

    void RemoveFirst()
    {
        if (count == 0)
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
        --count;
    }

    void RemoveLast()
    {
        if (count == 0)
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
        --count;
    }

    void RemoveAt(int32 index)
    {
        NodeType *node = FindNodeByIndex(index);
        CheckRange(node);
        RemoveNode(node);
    }

    bool RemoveValue(const Element &value)
    {
        NodeType *node = FindNode(value);
        if (node)
        {
            RemoveNode(node);
            return true;
        }
        return false;
    }

    Element &First()
    {
        NodeType *node = head;
        CheckRange(node);
        return node->GetValue();
    }

    const Element &First() const
    {
        const NodeType *node = head;
        CheckRange(node);
        return node->GetValue();
    }

    Element &Last()
    {
        NodeType *node = tail;
        CheckRange(node);
        return node->GetValue();
    }

    const Element &Last() const
    {
        const NodeType *node = tail;
        CheckRange(node);
        return node->GetValue();
    }

    void Pop()
    {
        RemoveLast();
    }

    Element &At(int32 index)
    {
        NodeType *node = FindNodeByIndex(index);
        CheckRange(node);
        return node->GetValue();
    }

    const Element &At(int32 index) const
    {
        const NodeType *node = FindNodeByIndex(index);
        CheckRange(node);
        return node->GetValue();
    }

    Element &operator[](int32 index)
    {
        return At(index);
    }

    const Element &operator[](int32 index) const
    {
        return At(index);
    }

    bool operator==(const List &other) const
    {
        if (count != other.count)
        {
            return false;
        }

        NodeType *ptr0 = head;
        NodeType *ptr1 = other.head;
        while (ptr0 && ptr1)
        {
            if (ptr0->GetValue() != ptr1->GetValue())
            {
                return false;
            }
            ptr0 = ptr0->GetNext();
            ptr1 = ptr1->GetNext();
        }
        return true;
    }

    bool operator!=(const List &other) const
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
            CT_CHECK(current);
            return current->GetValue();
        }

        Element *operator->()
        {
            CT_CHECK(current);
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
            CT_CHECK(current);
            return current->GetValue();
        }

        const Element *operator->()
        {
            CT_CHECK(current);
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
    void CheckRange(NodeType *node) const
    {
        CT_CHECK(node != nullptr);
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

    void AddFirstPrivate(NodeType *node)
    {
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
        ++count;
    }

    void AddLastPrivate(NodeType *node)
    {
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
        ++count;
    }

    void InsertPrivate(NodeType *before, NodeType *node)
    {
        if (before == nullptr || before == head)
        {
            AddFirstPrivate(node);
        }
        else
        {
            if (before->GetPrev())
            {
                node->SetPrev(before->GetPrev());
                before->GetPrev()->SetNext(node);
            }
            node->SetNext(before);
            before->SetPrev(node);
            ++count;
        }
    }

private:
    using NodeAlloc = Alloc<NodeType>;

    NodeType *head = nullptr;
    NodeType *tail = nullptr;
    int32 count = 0;
};

namespace std
{
template <typename E, template <typename T> class N, template <typename T> class A>
inline void swap(List<E, N, A> &lhs, List<E, N, A> &rhs)
{
    lhs.Swap(rhs);
}
}