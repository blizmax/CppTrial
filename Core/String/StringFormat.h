#pragma once

#include "Core/General.h"
#include "Core/String.h"

CT_SCOPE_BEGIN

namespace StringFormatInternal
{
    struct FormatParam
    {
        String stringValue;

        FormatParam(String &&str) : stringValue(std::move(str))
        {

        }

        const String& ToString() const
        {
            return stringValue;
        } 
    };

    template<typename T>
    String ToString(const T& input)
    {
        //TODO
        return (String)input;
    }

    void ParseParams(Array<FormatParam>& params)
    {

    }

    template <typename T, typename... Args>
    void ParseParams(Array<FormatParam>& params, T &&first, Args &&... args)
    {
        params.Add(FormatParam(ToString(first)));
        ParseParams(params, std::forward<Args>(args)...);
    }

    template <typename... Args>
    String FormatImpl(const String &src, Args &&... args)
    {
        const size_t paramSize = sizeof...(args); 
        if (paramSize == 0)
        {
            return src;
        }

        Array<FormatParam> params;
        ParseParams(params, std::forward<Args>(args)...);

        size_t pos = 0;
        auto cstr = src.GetPtr();
        //TODO use StringBuilder instead
        String output;

        // while (cstr[pos])
        // {
        //     if (cstr[pos++] == L'{')
        //     {
        //         if ()
        //     }
        // }

        for (const auto &param : params)
        {
            output += param.ToString();
        }

        return output;
    }
}

namespace StringFormat
{
    template <typename ...Args>
    String Format(const String& src, Args&&...args)
    {
        return StringFormatInternal::FormatImpl(src, std::forward<Args>(args)...);
    }
}

CT_SCOPE_END