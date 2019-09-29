#pragma once

#include "Core/General.h"
#include <string>

CT_SCOPE_BEGIN

namespace CString
{

typedef wchar CharType;
typedef std::char_traits<CharType> CharTraits;

CT_INLINE bool IsDigit(CharType input)
{
    return input >= L'0' && input <= L'9';
}

}


CT_SCOPE_END