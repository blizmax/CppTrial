#include <iostream>

#include "Core/Array.h"
#include "Core/HashSet.h"
#include "Core/HashMap.h"
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

    // HashSet<int> set;
       
    // for(int i = 50; i > 0; --i)
    // {
    //     set.Add(i * 50 + 17);
    // }

    // bool ret = set.Remove(67);

    // const HashSet<int> newSet = std::move(set);

    // HashSet<int> initSet = {1,77,150,77};
    // initSet = {99, 66};

    // for (const auto k : initSet)
    // {
    //     std::cout << k << std::endl;
    // }

    // HashSet<int> largeSet(5012);
    // largeSet.Shrink();

    // HashMap<int, int> map;
    
    // for(int i = 0; i < 100; ++i)
    // {
    //     map.Put(i, i * 100);
    // }

    // map[1] = 999;

    // for(auto e : map)
    // {
    //     std::cout << e.Key() << " : " << e.Value() << std::endl;
    // }

    HashMap<String, int> map2;
    for (int i = 0; i < 60; ++i)
    {
        String str = String(L"string") + L'A' + i;
        std::cout << str.HashCode() << std::endl;
        map2.Put(str, i);
    }

    for(auto e : map2)
    {
        std::wcout << *e.Key() << " : " << e.Value() << std::endl;
    }

    system("pause");
    return 0;
}