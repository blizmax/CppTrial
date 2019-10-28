#pragma once

#include "Reflection/QualifiedType.h"
#include "Reflection/Constructor.h"
#include "Reflection/Property.h"
#include "Reflection/Method.h"
#include "Reflection/Type.h"

namespace Reflection
{
class TestClass1
{
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

    static Type *GetType()
    {
        static Type type(L"TestClass1", nullptr, sizeof(TestClass1));
        return &type;
    }
};

void TestConstructor()
{
    // auto constructor1 = ConstructorImpl<TestClass1>();
    // TestClass1 *ptr1 = constructor1.Invoke();
    // ptr1->Print();

    // auto constructor2 = ConstructorImpl<TestClass1, const String &>();
    // TestClass1 *ptr2 = constructor2.Invoke(String(L"Obj2"));
    // ptr2->Print();

    // auto constructor3 = ConstructorImpl<TestClass1, const String &, int32>();
    // TestClass1 *ptr3 = constructor3.Invoke(String(L"Obj3"), 100);
    // ptr3->Print();

    // int32 ptrValue = 20;
    // auto constructor4 = ConstructorImpl<TestClass1, const String &, int32, int32 *>();
    // TestClass1 *ptr4 = constructor4.Invoke(String(L"Obj4"), 100, &ptrValue);
    // ptr4->Print();
}
} // namespace Reflection