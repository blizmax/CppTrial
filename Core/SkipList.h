#pragma once

#include "Core/.Package.h"
#include "Core/Allocator.h"

namespace SkipListInternal
{
template <typename Element>
class Node
{
public:
    Element element;
    Node **forward;
    uint32 level;
};
} // namespace SkipListInternal

template <typename Element, typename Compare, typename KeyTraits, template <typename> class Alloc>
class SkipList
{
public:
    static constexpr uint32 MAX_LEVEL = 16;

private:
    using Node = SkipListInternal::Node<Element>;

    Node *header = nullptr;
    Node *footer = nullptr;
    SizeType size = 0;
};