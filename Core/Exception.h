#pragma once

#include "Core/.Package.h"
#include "Core/String.h"

class Exception
{
public:
    Exception(const String &tag, const String &msg) : tag(tag), what(msg)
    {
    }

    Exception(const String &tag, const String &msg, const CharType *file, int32 line) : tag(tag), what(msg)
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

#define __CT_EXCEPTION_INNER(file) CT_TEXT(file)

#define CT_EXCEPTION(msg)                                                            \
    {                                                                                \
        Exception e(CT_TEXT("Core"), msg, __CT_EXCEPTION_INNER(__FILE__), __LINE__); \
        CT_DEBUG_BREAK();                                                            \
    }