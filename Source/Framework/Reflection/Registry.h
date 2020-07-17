#pragma once

#include "Reflection/Type.h"

namespace Reflection
{

struct ITypePopulator
{
    virtual void Populate() const = 0;
};

struct MetaData
{
    Name name;
    String value;
    MetaData(const Name &name, const String &value)
        : name(name), value(value)
    {
    }

    MetaData(const Name &name, String &&value)
        : name(name), value(std::move(value))
    {
    }

    void Apply(MetaBase *metaBase)
    {
        metaBase->SetMetaData(name, std::move(value));
    }
};

struct Parameter
{
    uint32 index;
    Name name;
    Any value;

    Parameter(uint32 index, const Name &name)
        : index(index), name(name)
    {
    }

    Parameter(uint32 index, const Name &name, const Any &value)
        : index(index), name(name), value(value)
    {
    }

    Parameter(uint32 index, const Name &name, Any &&value)
        : index(index), name(name), value(std::move(value))
    {
    }

    void Apply(MetaBase *metaBase)
    {
        if (dynamic_cast<Constructor *>(metaBase))
        {
            const ParamInfo &param = dynamic_cast<Constructor *>(metaBase)->GetParamInfos()[index];
            const_cast<ParamInfo &>(param).name = name;
            if (!value.IsEmpty())
            {
                const_cast<ParamInfo &>(param).defaultValue = std::move(value);
            }
        }
        else if (dynamic_cast<Method *>(metaBase))
        {
            const ParamInfo &param = dynamic_cast<Method *>(metaBase)->GetParamInfos()[index];
            const_cast<ParamInfo &>(param).name = name;
            if (!value.IsEmpty())
            {
                const_cast<ParamInfo &>(param).defaultValue = std::move(value);
            }
        }
    }
};

struct EnumValue
{
    Enum::Element element;

    template <typename T>
    EnumValue(const Name &name, T value)
        : element(name, static_cast<int64>(value))
    {
    }
};

template <typename T>
class TypeRegistrar
{
public:
    Type *type;
    MetaBase *metaBase;
    Array<Constructor *> constructors;
    Array<Property *> properties;
    Array<Method *> methods;
    Array<Enum *> enums;

    explicit TypeRegistrar()
    {
        type = TypeOf<T>();
        CT_ASSERT(!type->IsEnum());
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

    TypeRegistrar &operator()()
    {
        return *this;
    }

    template <typename ItemType, typename... Args>
    TypeRegistrar &operator()(ItemType &&item, Args &&... args)
    {
        CT_ASSERT(metaBase != nullptr);
        item.Apply(metaBase);
        return (*this)(std::forward<Args>(args)...);
    }
};

template <typename T>
class EnumRegistrar
{
public:
    Type *type;
    Array<Enum::Element> elements;

    explicit EnumRegistrar()
    {
        type = TypeOf<T>();
        CT_ASSERT(type->IsEnum());
    }

    void Apply()
    {
        Enum *e = type->GetEnum();
        e->elements = elements;
    }

    EnumRegistrar &Values()
    {
        return *this;
    }

    template <typename ValueType, typename... Args>
    EnumRegistrar &Values(ValueType &&value, Args &&... args)
    {
        elements.Add(value.element);
        return Values(std::forward<Args>(args)...);
    }
};

class Registry
{
private:
    Registry();
    ~Registry();

public:
    static Registry *GetInstance();

    Type *GetTypeByName(const Name &name);
    bool RegisterType(Type *type);
    bool RegisterPopulator(const Name &name, ITypePopulator *populator);
    void PopulateAllTypes();

private:
    void UnregisterType(Type *type);
    void PopulateType(Type *type);

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
        }                                                                                     \
        return type;                                                                          \
    }                                                                                         \
                                                                                              \
private:

#define CT_TYPE_DEFINE_MACRO_COMBINE_INNER(A_, B_) A_##B_
#define CT_TYPE_DEFINE_MACRO_COMBINE(A_, B_) CT_TYPE_DEFINE_MACRO_COMBINE_INNER(A_, B_)

#define CT_TYPE_DEFINE(TYPE_)                                                                                              \
    static void CT_TYPE_DEFINE_MACRO_COMBINE(_Populate, __LINE__)();                                                       \
    struct CT_TYPE_DEFINE_MACRO_COMBINE(TypePopulator, __LINE__) : public Reflection::ITypePopulator                       \
    {                                                                                                                      \
        CT_TYPE_DEFINE_MACRO_COMBINE(TypePopulator, __LINE__)                                                              \
        ()                                                                                                                 \
        {                                                                                                                  \
            Reflection::Registry::GetInstance()->RegisterPopulator(CT_TEXT(#TYPE_), this);                                 \
        }                                                                                                                  \
        virtual void Populate() const override                                                                             \
        {                                                                                                                  \
            CT_TYPE_DEFINE_MACRO_COMBINE(_Populate, __LINE__)                                                              \
            ();                                                                                                            \
        }                                                                                                                  \
    };                                                                                                                     \
    static const CT_TYPE_DEFINE_MACRO_COMBINE(TypePopulator, __LINE__) CT_TYPE_DEFINE_MACRO_COMBINE(_populator, __LINE__); \
    static void CT_TYPE_DEFINE_MACRO_COMBINE(_Populate, __LINE__)()

#define CT_ENUM_DECLARE(ENUM_)                                                                                                            \
    template <>                                                                                                                           \
    struct Reflection::TypeTraits<ENUM_>                                                                                                  \
    {                                                                                                                                     \
        static Reflection::Type *GetType()                                                                                                \
        {                                                                                                                                 \
            static Reflection::Type *type = nullptr;                                                                                      \
            if (!type)                                                                                                                    \
            {                                                                                                                             \
                Enum *e = Memory::New<Reflection::Enum>(CT_TEXT(#ENUM_), Reflection::Type::GetType<std::underlying_type<ENUM_>::type>()); \
                type = Memory::New<Reflection::Type>(e);                                                                                  \
                Reflection::Registry::GetInstance()->RegisterType(type);                                                                  \
            }                                                                                                                             \
            return type;                                                                                                                  \
        }                                                                                                                                 \
    };

#define CT_ENUM_DEFINE(ENUM_) CT_TYPE_DEFINE(ENUM_)

}