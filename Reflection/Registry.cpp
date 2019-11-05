#include "Reflection/Registry.h"

CT_SCOPE_BEGIN

namespace Reflection
{

Registry::Registry()
{
}

Registry::~Registry()
{
}

Registry *Registry::GetInstance()
{
    static Registry instance;
    return &instance;
}

bool Registry::RegisterType(Type *type)
{
    if (!typeMap.Contains(type->GetName()))
    {
        typeMap.Put(type->GetName(), type);

#if CT_REFLECTION_AUTO_POPULATE
        PopulateType(type);
#endif
        return true;
    }
    return false;
}

void Registry::UnregisterType(Type *type)
{
    typeMap.Remove(type->GetName());
}

Type *Registry::GetTypeByName(const Name& name)
{
    if(typeMap.Contains(name))
    {
        return typeMap[name];
    }
    return nullptr;
}

void Registry::PopulateType(Type *type)
{
    if (populatorMap.Contains(type->GetName()))
    {
        populatorMap[type->GetName()]->Populate();
    }
}

void Registry::PopulateAllTypes()
{
#if !(CT_REFLECTION_AUTO_POPULATE)
    for (auto& e : populatorMap)
    {
        e.Value()->Populate();
    }
#endif
}

bool Registry::RegisterPopulator(const Name &name, ITypePopulator *populator)
{
    if (!populatorMap.Contains(name))
    {
        populatorMap.Put(name, populator);
        return true;
    }
    return false;
}

} // namespace Reflection

CT_SCOPE_END