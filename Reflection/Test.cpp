#include "Reflection/Test.h"
#include <iostream>

void TestClass1::Print() const
{
    std::wcout << L"TestClass1 Name: " << *name << L" Num: " << num << std::endl;
    if (numPtr)
        std::wcout << L"Has ptr value: " << *numPtr << std::endl;
}

CT_ENUM_DEFINE(TestEnum1)
{
    using namespace Reflection;
    EnumRegistrar<TestEnum1>()
        .Values(
            EnumValue(CT_TEXT("E_A"), E_A),
            EnumValue(CT_TEXT("E_B"), E_B),
            EnumValue(CT_TEXT("E_C"), E_C),
            EnumValue(CT_TEXT("E_D"), E_D))
        .Apply();
}

CT_TYPE_DEFINE(TestClass1)
{
    using namespace Reflection;
    TypeRegistrar<TestClass1>()
        .AddConstructor<>()
        .AddConstructor<const String &>()(
            MetaData(CT_TEXT("Default Constructor"), CT_TEXT("")),
            Parameter(0, {CT_TEXT("name")}))
        .AddConstructor<const String &, int32>()
        .AddConstructor<const String &, int32, int32 *>()
        .AddProperty<String>(CT_TEXT("name"), &TestClass1::name)
        .AddProperty<int32>(CT_TEXT("num"), &TestClass1::num)
        .AddMethod<void>(CT_TEXT("Print"), &TestClass1::Print)
        .AddMethod<void, int32>(CT_TEXT("IncNum"), &TestClass1::IncNum)(
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
        std::wcout << L"IncNum param name: " << *p.GetName().ToString() << L" param type: " << *p.GetType().ToString() << std::endl;
    }
    auto printMethod = type->GetMethod(CT_TEXT("Print"));
    printMethod->Invoke(c1);

    Type *enumType = TypeOf<TestEnum1>();
    Enum *innerEnum = enumType->GetEnum();
    for (SizeType i = 0; i < innerEnum->GetElementSize(); ++i)
    {
        std::wcout << L"TestEnum1 values index:" << i << L" name: " << *innerEnum->GetNameByIndex(i).ToString() << L" value: " << innerEnum->GetValueByIndex(i) << std::endl;
    }

    Type *nestedEnumType = TypeOf<TestClass1::TestNestedEnum>();
    std::wcout << L"TestNestedEnum is enum? :" << enumType->IsEnum() << std::endl;
}

void Reflection::Test()
{
    Registry::GetInstance()->PopulateAllTypes();

    TestTypeMacro();
}