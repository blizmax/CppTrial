#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>

//#define CPP_TRIAL_NAME CppTrial

#ifdef CPP_TRIAL_NAME
#define CT_SCOPE_BEGIN       \
    namespace CPP_TRIAL_NAME \
    {
#define CT_SCOPE_END }
#define CT_SCOPE_USING using namespace CPP_TRIAL_NAME;
#else
#define CT_SCOPE_BEGIN
#define CT_SCOPE_END
#define CT_SCOPE_USING
#endif

#ifdef _DEBUG
#define CT_DEBUG 1
#endif

#define CT_ALIGN(X_, A_) (((X_) + ((A_)-1)) & ~((A_)-1))

#define CT_ASSERT(COND_) assert(COND_)

#ifdef CT_EXCEPTION
#define CT_THROW(MSG_) throw MSG_
#else
#define CT_THROW(...) CT_ASSERT(false)
#endif

#define CT_INLINE inline

CT_SCOPE_BEGIN

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef char char8;
typedef char16_t char16;
typedef char32_t char32;
typedef wchar_t wchar;

typedef wchar CharType;
typedef size_t SizeType;

static constexpr SizeType INDEX_NONE = static_cast<SizeType>(-1);

namespace ThisScope
{
}

CT_SCOPE_END