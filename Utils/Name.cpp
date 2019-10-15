#include "Name.h"
#include "Core/Memory.h"
#include "Core/HashMap.h"
#include "Core/Thread.h"

CT_SCOPE_BEGIN

static std::mutex mutex;
static HashMap<String, Name::Data *> nameMap(2048);

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

void Name::Construct(const String &str)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (nameMap.Contains(str))
    {
        Data *mapData = nameMap.Get(str);
        data = mapData;
    }
    else
    {
        Data *mapData = Memory::New<Data>();
        mapData->hash = Hash::HashValue(str);
        mapData->string = str;
        nameMap.Put(str, mapData);
        data = mapData;
    }
}

const Name::Data *Name::Find(const String &value)
{
    std::unique_lock<std::mutex> lock(mutex);

    if (nameMap.Contains(value))
    {
        return nameMap.Get(value);
    }
    return nullptr;
}

#ifdef CT_DEBUG
Array<Name::Data *> Name::DebugDumpNameMap()
{
    Array<Data *> names;

    std::unique_lock<std::mutex> lock(mutex);

    for (const auto &e : nameMap)
    {
        names.Add(e.Value());
    }
    return names;
}
#endif

CT_SCOPE_END