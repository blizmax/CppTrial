#include "Name.h"
#include "Core/Memory.h"
#include "Core/HashMap.h"
#include "Core/Thread.h"

static std::mutex mutex;

Name::Name(const Name &other) : data(other.data)
{
}

Name::Name(Name &&other) : data(other.data)
{
    other.data = nullptr;
}

Name &Name::operator=(const Name &other)
{
    if (this != &other)
    {
        Name temp(other);
        Swap(temp);
    }
    return *this;
}

Name &Name::operator=(Name &&other)
{
    if (this != &other)
    {
        Name temp(std::move(other));
        Swap(temp);
    }
    return *this;
}

Name::Name(const String &value)
{
    Construct(value);
}

Name::Name(const CharType *value)
{
    Construct(String(value));
}

static HashMap<String, Name::Data *>& NameMap()
{
    static HashMap<String, Name::Data *> nameMap(2048);
    return nameMap;
}

void Name::Construct(const String &str)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (NameMap().Contains(str))
    {
        Data *mapData = NameMap().Get(str);
        data = mapData;
    }
    else
    {
        Data *mapData = Memory::New<Data>();
        mapData->hash = Hash::HashValue(str);
        mapData->string = str;
        NameMap().Put(str, mapData);
        data = mapData;
    }
}

const Name::Data *Name::Find(const String &value)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (NameMap().Contains(value))
    {
        return NameMap().Get(value);
    }
    return nullptr;
}

#ifdef CT_DEBUG
Array<Name::Data *> Name::DebugDumpNameMap()
{
    Array<Data *> names;

    std::unique_lock<std::mutex> lock(mutex);

    for (const auto &e : NameMap())
    {
        names.Add(e.Value());
    }
    return names;
}
#endif