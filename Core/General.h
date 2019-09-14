#pragma once

#include <cassert>

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

#define CT_ALIGN(X_, A_) (((X_) + ((A_) - 1)) & ~((A_) - 1))

#define CT_ASSERT(COND_) assert(COND_)

#define CT_INLINE inline

CT_SCOPE_BEGIN

namespace ThisScope
{

}

CT_SCOPE_END