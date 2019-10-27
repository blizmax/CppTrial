#pragma once

#include "Reflection/.Package.h"

CT_SCOPE_BEGIN

namespace Reflection
{

class QualifiedType
{
public:
    QualifiedType() = default;

    QualifiedType(const Type *type) : type(type)
    {
    }

    QualifiedType(const QualifiedType &other, bool isPointer, bool isConst, bool isVolatile, bool isLVRef, bool isRVRef) : type(other.type), mask(other.mask), pointerCount(other.pointerCount)
    {
        if (isPointer)
            ++pointerCount;
        if (isConst)
            mask |= QTypeFlag_Const;
        if (isVolatile)
            mask |= QTypeFlag_Volatile;
        if (isLVRef)
            mask |= QTypeFlag_Refer;
        else if (isRVRef)
            mask |= QTypeFlag_RightRefer;
    }

    const Type *GetType() const
    {
        return type;
    }

    bool IsConst() const
    {
        return (mask & (1 << QTypeFlag_Const)) != 0;
    }

    bool IsVolatile() const
    {
        return (mask & (1 << QTypeFlag_Volatile)) != 0;
    }

    bool IsLValueReference() const
    {
        return (mask & (1 << QTypeFlag_Refer)) != 0;
    }

    bool IsRValueReference() const
    {
        return (mask & (1 << QTypeFlag_RightRefer)) != 0;
    }

    bool IsReference() const
    {
        return IsLValueReference() || IsRValueReference();
    }

    bool IsPointer() const
    {
        return pointerCount > 0;
    }

    uint32 PointerCount() const
    {
        return pointerCount;
    }

    QualifiedType RemoveCV() const
    {
        QualifiedType temp = *this;
        temp.mask &= ~(1 << QTypeFlag_Const);
        temp.mask &= ~(1 << QTypeFlag_Volatile);
        return temp;
    }

    QualifiedType RemoveReference() const
    {
        QualifiedType temp = *this;
        temp.mask &= ~(1 << QTypeFlag_Refer);
        temp.mask &= ~(1 << QTypeFlag_RightRefer);
        return temp;
    }

    QualifiedType RemovePointer() const
    {
        QualifiedType temp = *this;
        if (IsPointer())
            --temp.pointerCount;
        return temp;
    }

    bool CanCast(const QualifiedType &other) const;

    bool IsEnum() const;

    bool operator==(const QualifiedType &other) const
    {
        return type == other.type && mask == other.mask && pointerCount == other.pointerCount;
    }

    bool operator!=(const QualifiedType &other) const
    {
        return !(*this == other);
    }

protected:
    enum Flag
    {
        QTypeFlag_Const = 1,
        QTypeFlag_Volatile = 2,
        QTypeFlag_Refer = 4,
        QTypeFlag_RightRefer = 8,
    };
    typedef int32 FlagMask;

    const Type *type = nullptr;
    FlagMask mask = 0;
    uint32 pointerCount = 0;
};

template <typename T>
struct QualifiedTypeTraits
{
    static QualifiedType GetType()
    {
        return TypeOf<T>();
    }
};

template <typename T>
struct QualifiedTypeTraits<T &>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, false, false, true, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<T &&>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, false, false, false, true);
    }
};

template <typename T>
struct QualifiedTypeTraits<T *>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), true, false, false, false, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<const T>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, true, false, false, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<const T &>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, true, false, true, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<const T &&>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, true, false, false, true);
    }
};

template <typename T>
struct QualifiedTypeTraits<const T *>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), true, true, false, false, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<volatile T>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, false, true, false, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<volatile T &>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, false, true, true, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<volatile T &&>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, false, true, false, true);
    }
};

template <typename T>
struct QualifiedTypeTraits<volatile T *>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), true, false, true, false, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<const volatile T>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, true, true, false, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<const volatile T &>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, true, true, true, false);
    }
};

template <typename T>
struct QualifiedTypeTraits<const volatile T &&>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), false, true, true, false, true);
    }
};

template <typename T>
struct QualifiedTypeTraits<const volatile T *>
{
    static QualifiedType GetType()
    {
        return QualifiedType(QualifiedTypeTraits<T>::GetType(), true, true, true, false, false);
    }
};

template <typename T>
CT_INLINE QualifiedType GetQualifiedType()
{
    return QualifiedTypeTraits<T>::GetType();
}

} // namespace Reflection

CT_SCOPE_END