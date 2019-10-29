#pragma once

#include "Reflection/QualifiedType.h"
#include "Reflection/Constructor.h"
#include "Reflection/Property.h"
#include "Reflection/Method.h"
#include "Reflection/Type.h"
#include "Reflection/Registry.h"

CT_SCOPE_BEGIN

class TestClass1
{
    CT_TYPE_DECLARE(TestClass1, std::nullptr_t);

public:
    String name = L"None";
    int32 num = 0;
    int32 *numPtr = nullptr;

public:
    TestClass1() = default;

    TestClass1(const String &name) : name(name)
    {
    }

    TestClass1(const String &name, int32 num) : name(name), num(num)
    {
    }

    TestClass1(const String &name, int32 num, int32 *numPtr) : name(name), num(num), numPtr(numPtr)
    {
    }

    void Print() const
    {
        std::wcout << L"TestClass1 Name: " << *name << L" Num: " << num << std::endl;
        if (numPtr)
            std::wcout << L"Has ptr value: " << *numPtr << std::endl;
    }

    void IncNum(int32 inc)
    {
        num += inc;
    }

    const String& GetName() const
    {
        return name;
    }
};

CT_TYPE_DEFINE(TestClass1)
{
    Reflection::TypeRegistrar<TestClass1>()
    .AddConstructor<>()
    .AddConstructor<const String&>()
    .AddConstructor<const String&, int32>()
    .AddConstructor<const String&, int32, int32*>()
    .AddProperty<String>(CT_TEXT("name"), &TestClass1::name)
    .AddProperty<int32>(CT_TEXT("num"), &TestClass1::num)
    .AddMethod<void>(CT_TEXT("Print"), &TestClass1::Print)
    .AddMethod<void, int32>(CT_TEXT("IncNum"), &TestClass1::IncNum)
    .AddMethod<const String&>(CT_TEXT("GetName"), &TestClass1::GetName)
    .Apply();
}

namespace Reflection
{
void TestBuiltinType()
{
    // Type *type = TypeOf<int32>();
    // if (type)
    // {
    //     auto name = type->GetName();
    //     std::wcout << L"Type Name: " << *(name.ToString()) << std::endl;
    // }
}

void TestTypeMacro()
{
    Type *type = TypeOf<TestClass1>();
    auto ctor = type->GetConstructor();
    for(const auto prop : type->GetProperties())
    {
        std::wcout << L"property name: " << *prop->GetName().ToString() << std::endl;
    }
    for(const auto method : type->GetMethods())
    {
        std::wcout << L"method name: " << *method->GetName().ToString() << std::endl;
    }

    TestClass1 *c1 = ctor->Invoke();
    auto nameProp = type->GetProperty(CT_TEXT("name"));
    nameProp->Set(c1, String(CT_TEXT("Hello")));
    auto incNumMethod = type->GetMethod(CT_TEXT("IncNum"));
    incNumMethod->Invoke(c1, 100);
    auto printMethod = type->GetMethod(CT_TEXT("Print"));
    printMethod->Invoke(c1);
}

void Test()
{
   TestTypeMacro();
}
} // namespace Reflection
CT_SCOPE_END