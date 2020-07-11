#pragma once

#include "Core/Template/.Package.h"

template <typename T>
concept Stringable requires
{

};

template <typename T>
concept Hashable = requires(T a) 
{
    { a.HashCode() } -> std::same_as<uint32>;
};