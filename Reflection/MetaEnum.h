#pragma once

#include "Reflection/MetaType.h"

CT_SCOPE_BEGIN

class MetaEnum : public MetaType
{
public:
    struct Element
    {
        Name name;
        int64 value;
    };

public:
    MetaEnum() = default;
    virtual ~MetaEnum() = default;

    void SetElements(Array<Element> &value);
    SizeType GetElementSize() const;
    bool IsValidIndex(SizeType index) const;
    bool IsValidName(const Name &name) const;
    bool IsValidValue(int64 value) const;
    SizeType GetIndexByName(const Name &name) const;
    SizeType GetIndexByValue(int64 value) const;
    int64 GetValueByName(const Name &name) const;
    int64 GetValueByIndex(SizeType index) const;
    Name GetNameByValue(int64 value) const;
    Name GetNameByIndex(SizeType index) const;

protected:
    void CheckRange(SizeType index) const;

    Array<Element> elements;
};

template <typename T>
class MetaEnumRegistrar : public MetaTypeRegistrar
{
public:
    MetaEnumRegistrar(const Name &name) : MetaTypeRegistrar(name)
    {
        //TODO
    }
};

CT_SCOPE_END

#define CT_REFLECT_DECL_ENUM(ENUM_) \
public:
