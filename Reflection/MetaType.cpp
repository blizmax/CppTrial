#include "Reflection/MetaType.h"
#include "Core/List.h"

CT_SCOPE_BEGIN

static List<MetaTypeRegistrar *> registrarList;

MetaTypeRegistrar::MetaTypeRegistrar(const Name &name) : name(name)
{
}

void MetaTypeRegistrar::Register()
{
}

void MetaTypeRegistrar::Unregister()
{
}

void MetaTypeRegistrar::AddRegistrarToList(MetaTypeRegistrar *registrar)
{
    registrarList.Add(registrar);
}

void MetaTypeRegistrar::RemoveRegistrarFromList(MetaTypeRegistrar *registrar)
{
    registrarList.RemoveValue(registrar);
}

void MetaTypeRegistrar::AddTypeToRegistry(MetaType *type)
{
    //TODO
}

void MetaTypeRegistrar::RemoveTypeFromRegistry(MetaType *type)
{
    //TODO
}

void MetaTypeRegistrar::RegisterAllTypes()
{
    //TODO
}

void MetaTypeRegistrar::UnRegisterAllTypes()
{
    //TODO
}

CT_SCOPE_END