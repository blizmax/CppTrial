#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/HashMap.h"
#include "Core/Threading.h"

CT_SCOPE_BEGIN

class Name
{
public:
    struct Data
    {
        int32 hash;
        String string;
    };

public:
    Name() = default;
    ~Name() = default;   // So no need to remove existing name.

    Name(const Name& other);
    Name(Name&& other);
    Name& operator=(const Name& other);
    Name& operator=(Name&& other);

    Name(const String& str);
    Name(const CharType* cstr);

    bool IsEmpty() const
    {
        return data == nullptr;
    }

    void Swap(Name& other)
    {
        std::swap(data, other.data);
    }

    const Data* GetData() const
    {
        return data;
    }

    String ToString() const
    {
        return data ? data->string : String();
    }

    uint32 HashCode() const
    {
        return data ? data->hash : 0;
    }

    bool operator==(const Name& other) const
    {
        if(data)
            return data == other.data;
        return other.data == nullptr;
    }

    bool operator!=(const Name& other) const
    {
        return *this == other;
    }

#ifdef CT_DEBUG
    static Array<Data*> DumpNameMap();
#endif

private:
    void Construct(const String& str);

private:
    Data* data = nullptr;

    static std::mutex mutex;
    static HashMap<String, Data*> nameMap;
};

CT_SCOPE_END