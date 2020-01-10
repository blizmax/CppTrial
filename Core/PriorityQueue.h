#pragma once

#include "Core/Container/.Package.h"
#include "Core/Array.h"
#include "Core/Algorithm.h"

template<typename Element, typename InnerContainer = Array<Element>, typename Compare = Less<Element>>
class PriorityQueue
{
public:
    using ContainerType = InnerContainer;

    PriorityQueue() = default;
    PriorityQueue(const PriorityQueue &) = default;
    PriorityQueue(PriorityQueue &&) noexcept = default;
    PriorityQueue &operator=(const PriorityQueue &) = default;
    PriorityQueue &operator=(PriorityQueue &&) noexcept = default;
    ~PriorityQueue() = default;
 
    PriorityQueue(std::initializer_list<Element> initList)
    {
        for (const Element &value : initList)
        {
            Push(value);
        }
    }

    SizeType Size() const
    {
        return container.Size();
    }

    bool IsEmpty() const
    {
        return container.IsEmpty();
    }

    void Swap(PriorityQueue &other)
    {
        if (this != &other)
        {
            std::swap(container, other.container);
        }
    }

    void Clear()
    {
        container.Clear();
    }

    Element &First()
    {
        return container.First();
    }

    const Element &First() const
    {
        return container.First();
    }

    void Push(const Element &value)
    {
        container.Add(value);
        AlgoInternal::SiftUp(container.GetData(), Size(), Size() - 1, compare); 
    }

    void Push(Element &&value)
    {
        container.Add(std::move(value));
        AlgoInternal::SiftUp(container.GetData(), Size(), Size() - 1, compare); 
    }

    void Pop()
    {
        SizeType size = Size();
        if(size >= 2)
        {
            auto heap = container.GetData();
            std::swap(heap[0], heap[size - 1]);
        }
        if(size >= 1)
        {
            container.RemoveLast();
        }
        AlgoInternal::SiftDown(container.GetData(), Size(), 0, compare); 
    }

    bool operator==(const PriorityQueue &other) const
    {
        return container == other.container;
    }

    bool operator!=(const PriorityQueue &other) const
    {
        return container != other.container;
    }

private:
    ContainerType container;
    Compare compare;
};

namespace std
{
template <typename T, typename C>
inline void swap(PriorityQueue<T, C> &lhs, PriorityQueue<T, C> &rhs)
{
    lhs.Swap(rhs);
}
}