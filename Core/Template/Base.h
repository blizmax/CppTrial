#pragma once

#include "Core/General.h"
#include <type_traits>

CT_SCOPE_BEGIN

using TTrueType = std::true_type;
using TFalseType = std::false_type;

template<bool B, typename T>
using TEnableIf = std::enable_if<B, T>;

template<typename T>
using TIsVoid = std::is_void<T>;

template<typename T>
using TDecay = std::decay<T>;

template <typename T>
using TIsTriviallyDestructible = std::is_trivially_destructible<T>;
template <typename T>
using TIsTriviallyCopyAssignable = std::is_trivially_copy_assignable<T>;
template <typename T>
using TIsTriviallyMoveAssignable = std::is_trivially_move_assignable<T>;

CT_SCOPE_END