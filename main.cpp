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

    String shortStr = L'S';
    String temp = L"12345";
    temp.Replace(0, 2, L'Z', 3);
    std::wcout << *temp << std::endl;

    size_t pos = 0;
    bool ok = temp.Find(L'P', 0, pos);

    String str(longCharArr);
    String str1 = L"PQR";
    size_t pos1 = 0;
    bool ret = str.Find(*str1, pos1);

    system("pause");
    return 0;
}