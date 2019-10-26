#pragma once

#include "Reflection/MetaType.h"

CT_SCOPE_BEGIN

class Registry
{
private:
    Registry();
    ~Registry();

public:
    static Registry *GetInstance();

    bool RegisterType(const MetaType *type);
    void UnregisterType(const MetaType *type);

    const MetaType *GetMetaType(const Name &name) const;

    template <typename T>
    const MetaType *GetMetaType();

private:
    HashMap<Name, MetaType *> typeMap;
};

CT_SCOPE_END