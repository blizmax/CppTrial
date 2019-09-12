#include <iostream>

#include "Array.h"

class A
{
public:
    A() {std::cout << "A Constructor" << std::endl;}
};

int main()
{
    typedef Allocator<A>  AAllocator;
    A* ptr = AAllocator::Allocate();
    std::cout << "A Alloc" << std::endl;
    AAllocator::Construct(ptr);

    system("pause");
    return 0;
}