#include <iostream>

#include "Array.h"

class A
{
public:
    A() {std::cout << "A Constructor" << std::endl;}
};

int main()
{
    Array<int> arr(5);
    arr.Add(100);
    std::cout << arr[0] << " , " << arr.size << std::endl;

    system("pause");
    return 0;
}