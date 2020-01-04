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
            skipList.Add(key);
        }
    }

    SortedSet &operator=(std::initializer_list<Key> initList)
    {
        SortedSet temp(initList);
        Swap(temp);
        return *this;
    }

public:
    SizeType Size() const
    {
        return skipList.Size();
    }

    bool IsEmpty() const
    {
        return skipList.IsEmpty();
    }

    void Swap(SortedSet &other)
    {
        skipList.Swap(other.skipList);
    }

    void Clear()
    {
        skipList.Clear();
    }

    bool Contains(const Key &key) const
    {
        return skipList.ContainsKey(key);
    }

    bool Add(const Key &key)
    {
        return skipList.Add(key);
    }

    bool Add(Key &&key)
    {
        return skipList.Add(std::move(key));
    }

    const Key &Get(const Key &key) const
    {
        return skipList.GetByKey(key);
    }

    bool Remove(const Key &key)
    {
        return skipList.RemoveByKey(key);
    }

    bool operator==(const SortedSet &other) const
    {
        return skipList == other.skipList;
    }

    bool operator!=(const SortedSet &other) const
    {
        return !(*this == other);
    }

    //===================== STL STYLE =========================
public:
    auto begin()
    {
        return skipList.begin();
    }

    auto begin() const
    {
        return skipList.begin();
    }

    auto end()
    {
        return skipList.end();
    }

    auto end() const
    {
        return skipList.end();
    }

private:
    using KeyTraits = Container::SetKeyTraits<Key>;
    using SkipListType = SkipList<Key, Comparer, KeyTraits, Alloc>;

    SkipListType skipList;
};

namespace std
{
template <typename K, typename C, template <typename T> class A>
inline void swap(SortedSet<K, C, A> &lhs, SortedSet<K, C, A> &rhs)
{
    lhs.Swap(rhs);
}
}