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

Registry* Registry::GetInstance()
{
    static Registry instance;
    return &instance;
}

bool Registry::RegisterType(Type *type)
{
    if(!typeMap.Contains(type->GetName()))
    {
        typeMap.Put(type->GetName(), type);
        return true;
    }
    return false;
}

void Registry::UnregisterType(Type *type)
{
    typeMap.Remove(type->GetName());
}

}

CT_SCOPE_END