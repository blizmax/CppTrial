#pragma once

#include "Core/.Package.h"
#include "Core/Container/SkipList.h"

template <typename Key,
          typename Value,
          typename Comparer = SkipListInternal::Comparator<Key>,
          template <typename T> class Alloc = Allocator>
class SortedMap
{
};