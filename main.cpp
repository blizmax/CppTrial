#include <iostream>

#include "Core/Array.h"
#include "Core/HashTable.h"
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
    
    HashTable<int> hashTable;
    
    for(int i = 50; i > 0; --i)
    {
        hashTable.Put(i * 50 + 17);
    }

    size_t pos = hashTable.Find(50);

    for(auto k : hashTable)
    {
        std::cout << k << std::endl;
    }

    system("pause");
    return 0;
}