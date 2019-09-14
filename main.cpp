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
    // A tmp;
    // Array<A> arr(5);
    // arr.Add(tmp);
    // arr.Reserve(8);
    // std::cout << arr.capacity << " , " << arr.size << std::endl;

    Array<int> arr1 = {1,2,3,4,5,6,7};
    Array<int> arr2 = {1,2,3,4,5,6};
    arr1 = arr2;

    arr1.Reverse();

    std::cout << arr1.Peek() << std::endl;
    arr1.Pop();
    std::cout << arr1.Peek() << std::endl;

    for(auto v : arr1)
    {
        std::cout << v << std::endl;
    }

    String str = L"HHHHH";
    String str2 = str;
    String str3;
    std::wcout << *str3 << std::endl;

    system("pause");
    return 0;
}