#pragma once

#include "Reflection/Type.h"

CT_SCOPE_BEGIN

namespace Reflection
{

struct ITypePopulator
{
    virtual void Populate() const = 0;
};

template <typename T>
class TypeRegistrar
{
public:
    class Option
    {
    public:
        Option(TypeRegistrar *registrar, MetaBase *metaBase) 
            : registrar(registrar), metaBase(metaBase)
        {
        }

        Option& AddMetaData(const Name& name, const String& value)
        {
            metaBase->SetMetaData(name, value);
            return *this;
        }

        TypeRegistrar& operator()()
        {
            return *registrar;
        }

    private:
        TypeRegistrar *registrar;
        MetaBase *metaBase;
    };

public:
    Type *type;
    MetaBase *metaBase;
    Array<Constructor *> constructors;
    Array<Property *> properties;
    Array<Method *> methods;

    explicit TypeRegistrar()
    {
        type = TypeOf<T>();
    }

    void Apply()
    {
        type->SetConstructors(constructors);
        type->SetProperties(properties);
        type->SetMethods(methods);
    }

    template <typename... Args>
    TypeRegistrar &AddConstructor()
    {
        Constructor *ctor = Memory::New<ConstructorImpl<T, Args...>>();
        constructors.Add(ctor);
        metaBase = ctor;
        return *this;
    }

    template <typename PropertyType>
    TypeRegistrar &AddProperty(const Name &name, PropertyType T::*property)
    {
        Property *prop = Memory::New<MemberProperty<T, PropertyType>>(name, property);
        properties.Add(prop);
        metaBase = prop;
        return *this;
    }

    template <typename PropertyType>
    TypeRegistrar &AddReadonlyProperty(const Name &name, PropertyType T::*property)
    {
        Property *prop = Memory::New<ReadonlyProperty<T, PropertyType>>(name, property);
        properties.Add(prop);
        metaBase = prop;
        return *this;
    }

    template <typename PropertyType>
    TypeRegistrar &AddStaticProperty(const Name &name, PropertyType *property)
    {
        Property *prop = Memory::New<StaticProperty<PropertyType>>(name, property);
        properties.Add(prop);
        metaBase = prop;
        return *this;
    }

    template <typename ReturnType, typename... Args>
    TypeRegistrar &AddMethod(const Name &name, ReturnType (T::*fun)(Args...))
    {
        Method *method = Memory::New<MemberMethod<T, ReturnType, Args...>>(name, fun);
        methods.Add(method);
        metaBase = method;
        return *this;
    }

    template <typename ReturnType, typename... Args>
    TypeRegistrar &AddMethod(const Name &name, ReturnType (T::*fun)(Args...) const)
    {
        using F = ReturnType (T::*)(Args...);
        Method *method = Memory::New<MemberMethod<T, ReturnType, Args...>>(name, (F)fun);
        methods.Add(method);
        metaBase = method;
        return *this;
    }

    template <typename ReturnType, typename... Args>
    TypeRegistrar &AddStaticMethod(const Name &name, ReturnType (*fun)(Args...))
    {
        Method *method = Memory::New<StaticMethod<ReturnType, Args...>>(name, fun);
        methods.Add(method);
        metaBase = method;
        return *this;
    }

    Option operator()()
    {
        return Option(this, metaBase);
    }
};

class Registry
{
private:
    Registry();
    ~Registry();

public:
    static Registry *GetInstance();

    bool RegisterType(Type *type);
    void UnregisterType(Type *type);
    Type *GetTypeByName(const Name &name);

    void PopulateType(Type *type);
    bool RegisterPopulator(const Name &name, ITypePopulator *populator);

private:
    HashMap<Name, Type *> typeMap;
    HashMap<Name, ITypePopulator *> populatorMap;
};

template <>
struct TypeTraits<void>
{
    static Type *GetType()
    {
        static Type *type = nullptr;
        if (!type)
        {
            type = Memory::New<Type>(CT_TEXT("void"), nullptr, 0);
            Registry::GetInstance()->RegisterType(type);
        }
        return type;
    }
};

#define CT_REFLECTION_BUILTIN_TYPE_TRAITS(TYPE_)                                   \
    template <>                                                                    \
    struct TypeTraits<TYPE_>                                                       \
    {                                                                              \
        static Type *GetType()                                                     \
        {                                                                          \
            static Type *type = nullptr;                                           \
            if (!type)                                                             \
            {                                                                      \
                type = Memory::New<Type>(CT_TEXT(#TYPE_), nullptr, sizeof(TYPE_)); \
                Registry::GetInstance()->RegisterType(type);                       \
            }                                                                      \
            return type;                                                           \
        }                                                                          \
    };

CT_REFLECTION_BUILTIN_TYPE_TRAITS(int8);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(int16);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(int32);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(int64);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint8);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint16);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint32);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(uint64);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(char8);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(char16);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(char32);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(wchar);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(bool);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(float);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(double);
CT_REFLECTION_BUILTIN_TYPE_TRAITS(String);

#define CT_TYPE_DECLARE(TYPE_, BASE_)                                                         \
public:                                                                                       \
    using Type = Reflection::Type;                                                            \
    static Type *GetType()                                                                    \
    {                                                                                         \
        static Type *type = nullptr;                                                          \
        if (!type)                                                                            \
        {                                                                                     \
            type = Memory::New<Type>(CT_TEXT(#TYPE_), Type::GetType<BASE_>(), sizeof(TYPE_)); \
            Reflection::Registry::GetInstance()->RegisterType(type);                          \
            Reflection::Registry::GetInstance()->PopulateType(type);                          \
        }                                                                                     \
        return type;                                                                          \
    }                                                                                         \
                                                                                              \
private:

#define CT_TYPE_DEFINE(TYPE_)                                                              \
    static void _Populate##TYPE_();                                                        \
    struct TYPE_##TypePopulator : public Reflection::ITypePopulator                        \
    {                                                                                      \
        TYPE_##TypePopulator()                                                             \
        {                                                                                  \
            Reflection::Registry::GetInstance()->RegisterPopulator(CT_TEXT(#TYPE_), this); \
        }                                                                                  \
        virtual void Populate() const override                                             \
        {                                                                                  \
            _Populate##TYPE_();                                                            \
        }                                                                                  \
    };                                                                                     \
    static const TYPE_##TypePopulator _populator##TYPE_;                                   \
    static void _Populate##TYPE_()

} // namespace Reflection

CT_SCOPE_END