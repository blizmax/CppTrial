#include <iostream>

#include "Array.h"
#include <vector>

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

    Array<int> arr1 {1,2,3,4,5};
    arr1.Insert(3, 350);
    for(auto v : arr1)
    {
        std::cout << v << std::endl;
    }
    arr1.Remove(2);
    arr1.Remove(2);
    for (auto v : arr1)
    {
        std::cout << v << std::endl;
    }

    system("pause");
    return 0;
}