#include <iostream>

#include "Core/Array.h"
#include "Core/List.h"
#include "Core/HashSet.h"
#include "Core/HashMap.h"
#include "Core/String.h"
#include "Core/Algorithm.h"
#include <vector>

USE_CT_SCOPE

class A
{
public:
    A() { std::cout << "A constructor" << std::endl; }
    A(const A &rhs) { std::cout << "A copy constructor" << std::endl; }
    A(A &&rhs)
    noexcept { std::cout << "A move constructor" << std::endl; }
    A &operator=(const A &rhs)
    {
        std::cout << "A copy assign" << std::endl;
        return *this;
    }
    A &operator=(A &&rhs) noexcept
    {
        std::cout << "A move assign" << std::endl;
        return *this;
    }
    ~A() { std::cout << "A destructor" << std::endl; }
};

int main()
{
    // Array<int> arr = {1, 98, 34, 25, 19, 34, 1, 98, 34, 77, 999, 27, 100, 6, 28, 1888, 89, 9, 130};
    // //AlgoInternal::BubbleSort(arr.GetData(), arr.Size(), &AlgoInternal::Less<decltype(arr.First())>);
    // //AlgoInternal::SelectionSort(arr.GetData(), arr.Size(), &AlgoInternal::Less<>);
    // Algo::QuickSort(arr.GetData(), arr.Size());

    // for(auto v : arr)
    // {
    //     std::cout << v << ", ";
    // }

    // size_t index = Algo::BinarySearch(arr.GetData(), arr.Size(), 77, &AlgoInternal::Less<int>);
    // int v = arr[index];

    List<int> list;
    for (int i = 100; i < 120; ++i)
    {
        list.Add(i);
    }

    list.Insert(3, 9999);
    List<int> copyList = list;
    List<int> list2 = {38, 49, 56, 777};
    //copyList = list2;
    auto node = copyList.FindNode(111);
    copyList.RemoveNode(node);

    for (auto v : copyList)
    {
        std::cout << v << std::endl;
    }

    system("pause");
    return 0;
}