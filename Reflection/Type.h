#include "Reflection/.Pakage.h"
#include "Reflection/QualifiedType.h"

CT_SCOPE_BEGIN

class Type : public MetaBase
{

};



//TODO
namespace ReflectionInternal
{
template <>
struct TypeTraits<int32>
{
    static const Type *GetType()
    {
        static Type type;
        return &type;
    }
};

template <>
struct TypeTraits<String>
{
    static const Type *GetType()
    {
        static Type type;
        return &type;
    }
};
}

CT_SCOPE_END