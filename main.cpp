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
    // String str = L"123";
    // str += L"45";
    // String str1 = L"678";
    // str += str1;

    // str.Remove(1, 2);

    // std::wcout << *str << std::endl;

    Array<int> arr = {1,2,3,4,5};
    int nums[] = {100, 101};
    arr.Insert(2, nums, 2);
    for(auto v : arr)
    {
        std::cout << v << std::endl;
    }

    system("pause");
    return 0;
}