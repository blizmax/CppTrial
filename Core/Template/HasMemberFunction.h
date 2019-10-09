#pragma once

#include "Core/Template/Base.h"

#define CT_TEMPLATE_HAS_MEMBER_FUNCTION(FUNC_)                             \
    template <typename T>                                                  \
    struct THas##FUNC_                                                     \
    {                                                                      \
        template <typename U>                                              \
        static auto Check(U) -> typename TDecay<decltype(U::FUNC_)>::type; \
        static void Check(...);                                            \
        using type = decltype(Check(std::declval<T>()));                   \
        enum                                                               \
        {                                                                  \
            value = !TIsVoid<type>::value                                  \
        };                                                                 \
    };

CT_SCOPE_BEGIN

CT_TEMPLATE_HAS_MEMBER_FUNCTION(HashCode)

CT_SCOPE_END