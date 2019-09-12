#pragma once

#ifdef CPP_TRIAL_NAME
#define CT_SCOPE_BEGIN namespace CPP_TRIAL_NAME {
#define CT_TRIAL_SCOPE_END }
#else
#define CT_SCOPE_BEGIN
#define CT_SCOPE_END
#endif

#define CT_INLINE inline

CT_SCOPE_BEGIN



CT_SCOPE_END