#pragma once

#include "Core/General.h"
#include "Core/String.h"
#include "Core/Array.h"
#include "Core/HashMap.h"
#include "Utils/Name.h"

CT_SCOPE_BEGIN

class MetaData
{
public:
protected:
    HashMap<Name, String> dataMap;
};

class MetaType
{
protected:
    Name name;
    MetaData metaData;
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