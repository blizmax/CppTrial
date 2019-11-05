#pragma once

#include "Core/General.h"
#include "Core/String.h"

CT_SCOPE_BEGIN

class Exception
{
public:
    Exception(const CharType *tag, const String &msg) : tag(tag), what(msg)
    {
    }

    Exception(const CharType *tag, const String &msg, const CharType *file, int32 line) : tag(tag), what(msg)
    {
        where = String(file) + CT_TEXT(":") + StringConvert::ToString(line);
    }

    String ToString() const
    {
        String ret = tag + CT_TEXT(":") + what;
        if (!ret.IsEmpty())
        {
            ret += CT_TEXT(" in ");
            ret += where;
        }
        return ret;
    }

    Exception() = delete;
    Exception(const Exception &other) = default;
    Exception &operator=(const Exception &other) = default;
    virtual ~Exception() = default;

protected:
    String tag;
    String what;
    String where;
};

#define CT_EXCEPT_INNER(_FILE) CT_TEXT(_FILE)

#define CT_EXCEPT(TAG_, MSG_)                                         \
    {                                                                 \
        Exception e(TAG_, MSG_, CT_EXCEPT_INNER(__FILE__), __LINE__); \
        throw e;                                                      \
    }

CT_SCOPE_END