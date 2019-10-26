#pragma once

#include "Reflection/.Package.h"
#include "Reflection/ParamInfo.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class ConstructorBase : public MetaBase
{
protected:
    ConstructorBase() = delete;

    ConstructorBase(const Type *type, std::initializer_list<QualifiedType> paramTypes) : type(type)
    {
        uint32 index = 0;
        for (const auto &paramType : paramTypes)
        {
            paramInfos.Add(ParamInfo(paramType, index++));
        }
    }

public:
    const Type *GetType() const
    {
        return type;
    }

    SizeType GetParamCount() const
    {
        return paramInfos.Size();
    }

    const Array<ParamInfo> &GetParamInfos() const
    {
        return paramInfos;
    }

    virtual Any Invoke() const
    {
        CT_THROW("Not implement");
        return Any();
    }

    virtual Any Invoke(Any arg1) const
    {
        CT_THROW("Not implement");
        return Any();
    }

    virtual Any Invoke(Any arg1, Any arg2) const
    {
        CT_THROW("Not implement");
        return Any();
    }

    virtual Any Invoke(Any arg1, Any arg2, Any arg3) const
    {
        CT_THROW("Not implement");
        return Any();
    }

    virtual Any Invoke(Any arg1, Any arg2, Any arg3, Any arg4) const
    {
        CT_THROW("Not implement");
        return Any();
    }

    virtual Any Invoke(Any arg1, Any arg2, Any arg3, Any arg4, Any arg5) const
    {
        CT_THROW("Not implement");
        return Any();
    }

    virtual Any Invoke(Any arg1, Any arg2, Any arg3, Any arg4, Any arg5, Any arg6) const
    {
        CT_THROW("Not implement");
        return Any();
    }

protected:
    const Type *type;
    Array<ParamInfo> paramInfos;
};

template <typename T, typename... Args>
class Constructor : public ConstructorBase
{
public:
    Constructor() : ConstructorBase(TypeOf<T>(), {GetQualifiedType<Args>()...})
    {
        static_assert(TIsConstructible<T, Args...>::value, "Attempt to define an undeclared constructor.");
    }

    virtual Any Invoke(typename TAsType<Args, Any>::value... args) const override
    {
        return (Any)Memory::New<T>(std::forward<Args>((Args)args)...);
    }
};

} // namespace Reflection

CT_SCOPE_END