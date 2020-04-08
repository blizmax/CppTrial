#pragma once

#include "Core/.Package.h"
#include "Core/Container/SkipList.h"

template <typename Key,
          typename Comparer = CompareTo<Key>,
          template <typename T> class Alloc = Allocator>
class SortedSet
{
public:
    SortedSet() = default;
    SortedSet(const SortedSet &) = default;
    SortedSet(SortedSet &&) noexcept = default;
    SortedSet &operator=(const SortedSet &) = default;
    SortedSet &operator=(SortedSet &&) noexcept = default;
    ~SortedSet() = default;

    SortedSet(std::initializer_list<Key> initList)
    {
        for (const Key &key : initList)
        {
            container.Add(key);
        }
    }

    SortedSet &operator=(std::initializer_list<Key> initList)
    {
        SortedSet temp(initList);
        Swap(temp);
        return *this;
    }

public:
    int32 Count() const
    {
        return container.Count();
    }

    bool IsEmpty() const
    {
        return container.IsEmpty();
    }

    void Swap(SortedSet &other) noexcept
    {
        container.Swap(other.container);
    }

    void Clear()
    {
        container.Clear();
    }

    bool Contains(const Key &key) const
    {
        return container.ContainsKey(key);
    }

    bool Add(const Key &key)
    {
        return container.Add(key);
    }

    bool Add(Key &&key)
    {
        return container.Add(std::move(key));
    }

    bool Remove(const Key &key)
    {
        return container.RemoveByKey(key);
    }

    bool operator==(const SortedSet &other) const
    {
        return container == other.container;
    }

    bool operator!=(const SortedSet &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    auto begin()
    {
        return container.begin();
    }

    auto begin() const
    {
        return container.begin();
    }

    auto end()
    {
        return container.end();
    }

    auto end() const
    {
        return container.end();
    }

private:
    using KeyTraits = Container::SetKeyTraits<Key>;
    using ContainerType = SkipList<Key, Comparer, KeyTraits, Alloc>;

    ContainerType container;
};

namespace std
{
template <typename K, typename C, template <typename T> class A>
inline void swap(SortedSet<K, C, A> &lhs, SortedSet<K, C, A> &rhs)
{
    lhs.Swap(rhs);
}
}