#include <iostream>

#include "Core/Array.h"
#include "Core/HashSet.h"
#include "Core/HashMap.h"
#include "Core/String.h"
#include "Core/Sort.h"
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
    Array<int> arr = {1, 98, 34, 25, 19, 34, 1, 98, 34, 77, 999, 27, 100, 6, 28, 1888, 89, 9, 130};
    Algo::BubbleSort(arr.GetData(), arr.Size());
    //Algo::QuickSort(arr.GetData(), arr.Size());

    for(auto v : arr)
    {
        std::cout << v << ", ";
    }
  
    system("pause");
    return 0;
}