#include <iostream>

#include "Core/Array.h"
#include "Core/String.h"
#include <vector>

USE_CT_SCOPE

class A
{
public:
    A() {std::cout << "A constructor" << std::endl;}
    A(const A& rhs) {std::cout << "A copy constructor" << std::endl;}
    A(A&& rhs) noexcept {std::cout << "A move constructor" << std::endl;}
    A& operator=(const A& rhs) {std::cout << "A copy assign" << std::endl; return *this;}
    A& operator=(A&& rhs) noexcept {std::cout << "A move assign" << std::endl; return *this;}
    ~A() {std::cout << "A destructor" << std::endl;}
};

int main()
{
    auto longCharArr = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    String longStr = String(longCharArr);
    String str = L"123";
    String str1 = L"456";
    String str2 = longStr.SubString(3);
    longStr.Replace(2, longStr.Length(), L"000");
    longStr.Replace(2, 3, L"ZZZZZZZZZZ");

    std::wcout << *longStr << std::endl;

    
    system("pause");
    return 0;
}