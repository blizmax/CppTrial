#include "Name.h"
#include "Core/HashMap.h"
#include "Core/Threading.h"

CT_SCOPE_BEGIN

std::mutex Name::mutex;
HashMap<String, Name::Data*> Name::nameMap(2048);

Name::Name(const Name& other) : data(other.data)
{
}

Name::Name(Name&& other) : data(other.data)
{
    other.data = nullptr;
}

Name& Name::operator=(const Name& other)
{
    if(this != &other)
    {
        Name temp(other);
        Swap(temp);
    }   
    return *this;
}

Name& Name::operator=(Name&& other)
{
    if(this != &other)
    {
        Name temp(std::move(other));
        Swap(temp);
    }   
    return *this;
}

Name::Name(const String& str)
{
    Construct(str);
}

Name::Name(const CharType* cstr)
{
    Construct(String(cstr));
}

void Name::Construct(const String& str)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (nameMap.Contains(str))
    {
        Data* mapData = nameMap.Get(str);
        data = mapData;
    }
    else
    {
        Data* mapData = new Data;
        mapData->hash = Hash::HashValue(str);
        mapData->string = str;
        nameMap.Put(str, mapData);
        data = mapData;
    }
}

#ifdef CT_DEBUG
    Array<Name::Data*> Name::DumpNameMap()
    {
        Array<Data*> names;

        std::unique_lock<std::mutex> lock(mutex);

        for(const auto& e : nameMap)
        {
            names.Add(e.Value());
        }
        return names;
    }
#endif


CT_SCOPE_END