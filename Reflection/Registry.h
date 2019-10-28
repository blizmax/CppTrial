#pragma once

#include "Reflection/Type.h"

CT_SCOPE_BEGIN

namespace Reflection
{
class Registry
{
private:
    Registry();
    ~Registry();

public:
    static Registry *GetInstance();

    bool RegisterType(Type *type);
    void UnregisterType(Type *type);

    // const MetaType *GetMetaType(const Name &name) const;

    // template <typename T>
    // const MetaType *GetMetaType();

private:
    HashMap<Name, Type *> typeMap;
};
}

CT_SCOPE_END