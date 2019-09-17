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

void add(int v)
{
    std::cout << "aaaaaa" << std::endl;
}
void add(int* r)
{
    std::cout << "bbbbbb" << std::endl;
}

int main()
{
    auto longCharArr = L" A B CDEFGHIJ KLMNOPQR  STUVWX YZ ";

    String shortStr = L'S';
    String temp = L"12345";
    temp.Replace(0, 2, L'Z', 3);
    //std::wcout << *temp << std::endl;

    String str(longCharArr);
    Array<String> subs = str.Split(L" ");
    for (auto sub : subs)
    {
        std::wcout << *sub << std::endl;
    }
    std::wcout << "========";

    str.ReplaceAll(L" ", L"$");
    std::wcout << *str;

    system("pause");
    return 0;
}