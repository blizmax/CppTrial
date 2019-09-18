#pragma once

#include <cassert>
#include <cstdint>

#ifdef CPP_TRIAL_NAME
#define CT_SCOPE_BEGIN       \
    namespace CPP_TRIAL_NAME \
    {
#define CT_SCOPE_END }
#define USE_CT_SCOPE using namespace CPP_TRIAL_NAME;
#else
#define CT_SCOPE_BEGIN
#define CT_SCOPE_END
#define USE_CT_SCOPE
#endif

#define CT_ALIGN(X_, A_) (((X_) + ((A_)-1)) & ~((A_)-1))

#define CT_ASSERT(COND_) assert(COND_)

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

static constexpr size_t INDEX_NONE = static_cast<size_t>(-1);

namespace ThisScope
{
}

CT_SCOPE_END