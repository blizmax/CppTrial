#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/String/StringConvert.h"

CT_SCOPE_BEGIN

namespace StringFormatInternal
{
struct FormatParam
{
    String stringValue;

    FormatParam(String &&str) : stringValue(std::move(str))
    {
    }

    const String &ToString() const
    {
        return stringValue;
    }
};

CT_INLINE void ParseParams(Array<FormatParam> &params)
{
}

template <typename T, typename... Args>
CT_INLINE void ParseParams(Array<FormatParam> &params, T &&first, Args &&... args)
{
    params.Add(FormatParam(StringConvert::ToString(first)));
    ParseParams(params, std::forward<Args>(args)...);
}

template <typename... Args>
CT_INLINE String FormatImpl(const String &src, Args &&... args)
{
    const SizeType paramSize = sizeof...(args);
    if (paramSize == 0)
    {
        return src;
    }

    Array<FormatParam> params;
    ParseParams(params, std::forward<Args>(args)...);

    SizeType pos0 = 0, pos1 = 0, pos2 = 0;
    auto cstr = src.GetPtr();
    String output;

    while (cstr[pos1])
    {
        if (cstr[pos1++] == L'{')
        {
            pos2 = pos1;
            while (cstr[pos2])
            {
                if (cstr[pos2++] == L'}')
                {
                    String key = String(cstr + pos1, pos2 - pos1 - 1);
                    //Current key only support param index
                    uint32 index;
                    if (StringConvert::TryParseUInt32(key, index) && (index < paramSize))
                    {
                        output += String(cstr + pos0, pos1 - pos0 - 1);
                        output += params[index].ToString();
                        pos0 = pos1 = pos2;
                    }
                    else
                    {
                        pos1 = pos2;
                    }
                    break;
                }
            }
        }
    }
    output += String(cstr + pos0, pos1 - pos0);
    return output;
}
} // namespace StringFormatInternal

namespace StringFormat
{
template <typename... Args>
CT_INLINE String Format(const String &src, Args &&... args)
{
    return StringFormatInternal::FormatImpl(src, std::forward<Args>(args)...);
}
} // namespace StringFormat

CT_SCOPE_END