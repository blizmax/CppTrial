#pragma once

#include "Core/.Package.h"
#include "Core/List.h"

template <typename Element, typename InnerContainer = List<Element>>
class Queue
{
public:
    using ContainerType = InnerContainer;

public:
    Queue() = default;
    Queue(const Queue &) = default;
    Queue(Queue &&) noexcept = default;
    Queue &operator=(const Queue &) = default;
    Queue &operator=(Queue &&) = default;
    ~Queue() = default;

    Queue(std::initializer_list<Element> initList)
    {
        for (const Element &value : initList)
        {
            container.Add(value);
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

    void Swap(Queue &other)
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

    Element &Last()
    {
        return container.Last();
    }

    const Element &Last() const
    {
        return container.Last();
    }

    void Enqueue(const Element &value)
    {
        container.Add(value);
    }

    void Enqueue(Element &&value)
    {
        container.Add(std::move(value));
    }

    void Dequeue()
    {
        container.RemoveFirst();
    }

    bool operator==(const Queue &other) const
    {
        return container == other.container;
    }

    bool operator!=(const Queue &other) const
    {
        return container != other.container;
    }

private:
    ContainerType container;
}

namespace std
{
template <typename T, typename C>
inline void swap(Queue<T, C> &lhs, Queue<T, C> &rhs)
{
    lhs.Swap(rhs);
}
}