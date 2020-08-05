#pragma once

#include "Core/.Package.h"
#include "Core/Array.h"

template <typename Element, typename InnerContainer = Array<Element>>
class Stack
{
public:
    using ContainerType = InnerContainer;

public:
    Stack() = default;
    Stack(const Stack &) = default;
    Stack(Stack &&) noexcept = default;
    Stack &operator=(const Stack &) = default;
    Stack &operator=(Stack &&) noexcept = default;
    ~Stack() = default;

    Stack(std::initializer_list<Element> initList)
    {
        for (const Element &value : initList)
        {
            container.Add(value);
        }
    }

    int32 Count() const
    {
        return container.Count();
    }

    bool IsEmpty() const
    {
        return container.IsEmpty();
    }

    void Swap(Stack &other) noexcept
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

    Element &Top()
    {
        return container.Last();
    }

    const Element &Top() const
    {
        return container.Last();
    }

    Element &Peek()
    {
        return container.Last();
    }

    const Element &Peek() const
    {
        return container.Last();
    }

    void Pop()
    {
        container.Pop();
    }

    void Push(const Element &value)
    {
        container.Add(value);
    }

    void Push(Element &&value)
    {
        container.Add(std::move(value));
    }

    bool operator==(const Stack &other) const
    {
        return container == other.container;
    }

    bool operator!=(const Stack &other) const
    {
        return container != other.container;
    }

private:
    ContainerType container;
};

namespace std
{
template <typename T, typename C>
inline void swap(Stack<T, C> &lhs, Stack<T, C> &rhs)
{
    lhs.Swap(rhs);
}
}