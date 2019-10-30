#pragma once

#include "Reflection/Type.h"

CT_SCOPE_BEGIN

namespace Reflection
{

struct ITypePopulator
{
    virtual void Populate() const = 0;
};

struct IOption
{
};

struct MetaData : public IOption
{
    Name name;
    String value;
    MetaData(const Name &name, const String &value) : name(name), value(value)
    {
    }

    MetaData(const Name &name, String &&value) : name(name), value(std::move(value))
    {
    }

    void Apply(MetaBase *metaBase)
    {
        metaBase->SetMetaData(name, std::move(value));
    }
};

struct Parameter : public IOption
{
    uint32 index;
    Name name;
    Any value;

    Parameter(uint32 index, const Name &name) : index(index), name(name)
    {
    }

    Parameter(uint32 index, const Name &name, const Any &value) : index(index), name(name), value(value)
    {
    }

    Parameter(uint32 index, const Name &name, Any &&value) : index(index), name(name), value(std::move(value))
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

struct EnumValue : public IOption
{
};

template <typename T>
class TypeRegistrar
{
public:
    class OptionBuilder
    {
    public:
        OptionBuilder(TypeRegistrar *registrar, MetaBase *metaBase)
            : registrar(registrar), metaBase(metaBase)
        {
        }

        TypeRegistrar &operator()()
        {
            return *registrar;
        }

        template <typename OptionType, typename... Args>
        TypeRegistrar &operator()(OptionType &&option, Args &&... args)
        {
            option.Apply(metaBase);
            return (*this)(std::forward<Args>(args)...);
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
    Array<Enum *> enums;

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

    template <typename EnumType>
    TypeRegistrar &AddEnum(const Name &name)
    {
        Enum *e = Memory::New<Enum>(name, TypeOf<std::underlying_type<EnumType>::type>());
        enums.Add(e);
        metaBase = e;
        return *this;
    }

    TypeRegistrar &AddEnum(const Name &name)
    {
        Enum *e = Memory::New<Enum>(name, TypeOf<uint32>());
        enums.Add(e);
        metaBase = e;
        return *this;
    }

    OptionBuilder Options()
    {
        CT_ASSERT(metaBase != nullptr);
        return OptionBuilder(this, metaBase);
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

#define CT_ENUM_DECLARE(ENUM_)                                                                                                            \
    template <>                                                                                                                           \
    struct Reflection::TypeTraits<ENUM_>                                                                                                  \
    {                                                                                                                                     \
        static Reflection::Type *                                                                                                         \
        GetType()                                                                                                                         \
        {                                                                                                                                 \
            static Reflection::Type *type = nullptr;                                                                                      \
            if (!type)                                                                                                                    \
            {                                                                                                                             \
                Enum *e = Memory::New<Reflection::Enum>(CT_TEXT(#ENUM_), Reflection::Type::GetType<std::underlying_type<ENUM_>::type>()); \
                type = Memory::New<Reflection::Type>(e);                                                                                  \
                Reflection::Registry::GetInstance()->RegisterType(type);                                                                  \
                Reflection::Registry::GetInstance()->PopulateType(type);                                                                  \
            }                                                                                                                             \
            return type;                                                                                                                  \
        }                                                                                                                                 \
    };

#define CT_ENUM_DEFINE(ENUM_) CT_TYPE_DEFINE(ENUM_)

#define CT_NESTED_ENUM_DECLARE(ENUM_)                                                                                                     \
    template <>                                                                                                                           \
    struct Reflection::TypeTraits<ENUM_>                                                                                                  \
    {                                                                                                                                     \
        static Reflection::Type *                                                                                                         \
        GetType()                                                                                                                         \
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

} // namespace Reflection

CT_SCOPE_END