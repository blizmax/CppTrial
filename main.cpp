#include "Core/Array.h"
#include "Core/List.h"
#include "Core/HashSet.h"
#include "Core/HashMap.h"
#include "Core/String.h"
#include "Core/Algorithm.h"
#include "Core/String/StringEncode.h"
#include "Core/String/StringConvert.h"
#include "Core/Log.h"
#include "Core/FileSystem.h"
#include "Core/Threading.h"
#include "Core/Time.h"

USE_CT_SCOPE

Log logger = Log(L"Main");

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

void TestArraySort()
{
    Array<int> arr = {1, 98, 34, 25, 19, 34, 1, 98, 34, 77, 999, 27, 100, 6, 28, 1888, 89, 9, 130};
    for (int i = 10000; i >= 0; --i)
    {
        arr.Add(i);
    }

    //AlgoInternal::BubbleSort(arr.GetData(), arr.Size(), &AlgoInternal::Less<decltype(arr.First())>);
    //AlgoInternal::SelectionSort(arr.GetData(), arr.Size(), &AlgoInternal::Less<>);
    //Algo::QuickSort(arr.GetData(), arr.Size());
    arr.Sort();

    for(auto v : arr)
    {
        std::cout << v << ", ";
    }
    std::cout << std::endl;

    for(SizeType i = 1; i < arr.Size(); ++i)
    {
        if(arr[i - 1] > arr[i])
        {
            logger.Error(L"Sort error at pos {0}, value is {1}", i, arr[i]);
        }
    }
}

void TestHashMap()
{
    HashMap<String, int> map1;
    map1.Put(L'A', 100);

    HashMap<int, int> map2;
    map2.Put(100, 100);

    HashMap<SizeType, int> map3;
    map3.Put(100, 100);

    HashMap<float, int> map4;
    map4.Put(12.10f, 100);

    HashMap<const char8*, int> map5;
    map5.Put("abc", 1);

    A* a = new A();
    HashMap<A *, int> map6;
    map6.Put(a, 1);
}

void TestStringEncode()
{
    String str1 = StringEncode::FromUTF8("😊😡/(ㄒoㄒ)/~~🐷");
    String str2 = String(L"😊😡/(ㄒoㄒ)/~~🐷");

    std::cout << (str1 == str2) << std::endl;
}

void TestStringConvert()
{
    float ret;
    bool ok = StringConvert::TryParseFloat(L"-.50", ret);
}

void TestLogger()
{
    logger.SetLevel(LogLevel::Info);
    logger.Info(L"IIIIII");
    logger.Error(L"{}{0}and{1}", String(L"String1"), sizeof(double));
}

void TestPath()
{
    FileSystem::Path path = FileSystem::Path(L"D:Parent");
    path.Append(FileSystem::Path(L"pic1.jpg"));
    logger.Info(path.GetExtension());
    logger.Info(path.GetFileNameWithoutExtension());
    logger.Info(path.GetFileName());
    logger.Info(path.GetFullPath());
}

void TestTimeUtils()
{
    logger.Info(L"Now: {0}", Time::NanoTime() / 1000000000);
    std::this_thread::sleep_for(Time::Seconds(1));
    logger.Info(L"Now: {0}", Time::MilliTime() / 1000);
    logger.Info(L"Next: {0}", Time::MilliTime(Time::Now() + Time::Seconds(1)) / 1000);
}

int main()
{
    // SizeType index = Algo::BinarySearch(arr.GetData(), arr.Size(), 77, &AlgoInternal::Less<int>);
    // int v = arr[index];

    // uint32 uniCode[] = {0x4E25, 0};
    // uint8 buffer[4] = {0};
    // SizeType count = StringConvert::UTF32ToUTF8(uniCode, uniCode + 1, buffer);
    // uint32 ret;
    // count = StringConvert::UTF8ToUTF32(buffer, buffer + count, &ret);

    // TestArraySort();
    // TestHashMap();

    TestTimeUtils();

    // std::thread thread1 = std::thread([](){
    //     Log log1 = Log(L"Thread1");
    //     log1.Info(L"Thread start.");

    //     log1.Info(L"Thread finish.");
    // });
    // thread1.join();



    system("pause");
    return 0;
}