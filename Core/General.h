#pragma once

#ifdef CPP_TRIAL_NAME
#define CPP_TRIAL_SCOPE_BEGIN namespace CPP_TRIAL_NAME {
#define CPP_TRIAL_SCOPE_END }
#else
#define CPP_TRIAL_SCOPE_BEGIN
#define CPP_TRIAL_SCOPE_END
#endif