#pragma once

#include "Reflection/.Package.h"
#include "Reflection/ParamInfo.h"

namespace Reflection
{

class Method : public MetaBase
{
protected:
    Method(const Name &name, Type *ownerType, const QualifiedType returnType, std::initializer_list<QualifiedType> paramTypes, bool isStatic = false)
        : MetaBase(name), ownerType(ownerType), returnType(returnType), isStatic(isStatic)
    {
        uint32 index = 0;
        for (const auto &paramType : paramTypes)
        {
            paramInfos.Add(ParamInfo(paramType, index++));
        }
    }

public:
    Type *GetOwnerType() const
    {
        return ownerType;
    }

    QualifiedType GetReturnType() const
    {
        return returnType;
    }

    bool IsStatic() const
    {
        return isStatic;
    }

    SizeType GetParamCount() const
    {
        return paramInfos.Size();
    }

    const Array<ParamInfo> &GetParamInfos() const
    {
        return paramInfos;
    }

    virtual Any Invoke(Any obj) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

    virtual Any Invoke(Any obj, Any arg1) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

    virtual Any Invoke(Any obj, Any arg1, Any arg2) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

    virtual Any Invoke(Any obj, Any arg1, Any arg2, Any arg3) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

    virtual Any Invoke(Any obj, Any arg1, Any arg2, Any arg3, Any arg4) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

    virtual Any Invoke(Any obj, Any arg1, Any arg2, Any arg3, Any arg4, Any arg5) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

    virtual Any Invoke(Any obj, Any arg1, Any arg2, Any arg3, Any arg4, Any arg5, Any arg6) const
    {
        CT_EXCEPTION("Not implement");
        return Any();
    }

protected:
    Type *ownerType;
    QualifiedType returnType;
    Array<ParamInfo> paramInfos;
    bool isStatic;
};

template <typename OwnerType, typename ReturnType, typename... Args>
class MemberMethod : public Method
{
public:
    using FuncPtr = ReturnType (OwnerType::*)(Args...);

    MemberMethod(const Name &name, FuncPtr func)
        : Method(name, TypeOf<OwnerType>(), GetQualifiedType<ReturnType>(), {GetQualifiedType<Args>()...}), funcPtr(func)
    {
    }

    virtual Any Invoke(Any obj, typename TAsType<Args, Any>::value... args) const override
    {
        return (Any)(((OwnerType *)obj)->*funcPtr)((Args)args...);
    }

private:
    FuncPtr funcPtr;
};

template <typename OwnerType, typename... Args>
class MemberMethod<OwnerType, void, Args...> : public Method
{
public:
    using FuncPtr = void (OwnerType::*)(Args...);

    MemberMethod(const Name &name, FuncPtr func)
        : Method(name, TypeOf<OwnerType>(), GetQualifiedType<void>(), {GetQualifiedType<Args>()...}), funcPtr(func)
    {
    }

    virtual Any Invoke(Any obj, typename TAsType<Args, Any>::value... args) const override
    {
        (((OwnerType *)obj)->*funcPtr)((Args)args...);
        return Any();
    }

private:
    FuncPtr funcPtr;
};

template <typename ReturnType, typename... Args>
class StaticMethod : public Method
{
public:
    using FuncPtr = ReturnType (*)(Args...);

    StaticMethod(const Name &name, FuncPtr func)
        : Method(name, nullptr, GetQualifiedType<ReturnType>(), {GetQualifiedType<Args>()...}, true), funcPtr(func)
    {
    }

    virtual Any Invoke(Any obj, typename TAsType<Args, Any>::value... args) const override
    {
        return funcPtr((Args)args...);
    }

private:
    FuncPtr funcPtr;
};

template <typename... Args>
class StaticMethod<void, Args...> : public Method
{
public:
    using FuncPtr = void (*)(Args...);

    StaticMethod(const Name &name, FuncPtr func)
        : Method(name, nullptr, GetQualifiedType<void>(), {GetQualifiedType<Args>()...}, true), funcPtr(func)
    {
    }

    virtual Any Invoke(Any obj, typename TAsType<Args, Any>::value... args) const override
    {
        funcPtr((Args)args...);
        return Any();
    }

private:
    FuncPtr funcPtr;
};

} // namespace Reflection