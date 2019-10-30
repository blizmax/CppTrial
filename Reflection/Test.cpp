#include "Reflection/Test.h"
#include <iostream>

CT_SCOPE_BEGIN

void TestClass1::Print() const
{
    std::wcout << L"TestClass1 Name: " << *name << L" Num: " << num << std::endl;
    if (numPtr)
        std::wcout << L"Has ptr value: " << *numPtr << std::endl;
}

CT_TYPE_DEFINE(TestClass1)
{
    using namespace Reflection;
    TypeRegistrar<TestClass1>()
        .AddConstructor<>()
        .AddConstructor<const String &>()
        .Options()(
            MetaData(CT_TEXT("Default Constructor"), CT_TEXT("")),
            Parameter(0, {CT_TEXT("name")}))
        .AddConstructor<const String &, int32>()
        .AddConstructor<const String &, int32, int32 *>()
        .AddProperty<String>(CT_TEXT("name"), &TestClass1::name)
        .AddProperty<int32>(CT_TEXT("num"), &TestClass1::num)
        .AddMethod<void>(CT_TEXT("Print"), &TestClass1::Print)
        .AddMethod<void, int32>(CT_TEXT("IncNum"), &TestClass1::IncNum)
        .Options()(
            Parameter(0, {CT_TEXT("inc")}))
        .AddMethod<const String &>(CT_TEXT("GetName"), &TestClass1::GetName)
        .Apply();
}

void Reflection::TestBuiltinType()
{
}

void Reflection::TestTypeMacro()
{
    Type *type = TypeOf<TestClass1>();
    auto ctor = type->GetConstructor();
    for (const auto prop : type->GetProperties())
    {
        std::wcout << L"property name: " << *prop->GetName().ToString() << std::endl;
    }
    for (const auto method : type->GetMethods())
    {
        std::wcout << L"method name: " << *method->GetName().ToString() << std::endl;
    }

    TestClass1 *c1 = ctor->Invoke();
    auto nameProp = type->GetProperty(CT_TEXT("name"));
    nameProp->Set(c1, String(CT_TEXT("Hello")));
    auto incNumMethod = type->GetMethod(CT_TEXT("IncNum"));
    incNumMethod->Invoke(c1, 100);
    for (auto p : incNumMethod->GetParamInfos())
    {
        std::wcout << L"IncNum param name: " << *p.GetName().ToString() << std::endl;
    }
    auto printMethod = type->GetMethod(CT_TEXT("Print"));
    printMethod->Invoke(c1);
}

void Reflection::Test()
{
    TestTypeMacro();
}

CT_SCOPE_END