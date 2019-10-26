#pragma once

#include "Reflection/.Package.h"

CT_SCOPE_BEGIN

class MetaType
{
public:
    MetaType(const Name &name) : name(name)
    {
    }

    Name name;
    Reflection::MetaData metaData;
};

class MetaTypeRegistrar
{
protected:
    MetaTypeRegistrar() = default;
    virtual ~MetaTypeRegistrar() = default;

public:
    MetaTypeRegistrar(const Name &name);

    virtual void Register();
    virtual void Unregister();

    static void AddRegistrarToList(MetaTypeRegistrar *registrar);
    static void RemoveRegistrarFromList(MetaTypeRegistrar *registrar);

    static void AddTypeToRegistry(MetaType *type);
    static void RemoveTypeFromRegistry(MetaType *type);

    static void RegisterAllTypes();
    static void UnRegisterAllTypes();

private:
    Name name;
};

CT_SCOPE_END