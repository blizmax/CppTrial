#include "Core/Math.h"
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
#include "Core/Template.h"
#include "Core/Variant.h"
#include "Core/Any.h"
#include "Math/Random.h"
#include "Math/UUID.h"

CT_SCOPE_USING

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

void TestMath()
{
    int i1 = Math::TruncToInt(-5.6f);
    float f1 = Math::Abs(-5.0);
    int i2 = Math::Abs(-15);
    float f2 = Math::Exp(1.0);
    float f3 = Math::Fmod(100, 99.5f);
    float f4 = Math::Log2(1024.0f);
    double f5 = Math::Pow(5.0, 4.0);
    float f6 = Math::Exp2(8.0f);
    float f7 = Math::Round(3.3f);
    float f8 = Math::Round(3.5f);
    auto min = Math::Min(100, 10, -5.0, 22, -33.5f);
    auto clamp = Math::Clamp(50.0, 77.0, 80.0);

    // for(int32 i = 0; i < 100; ++i)
    // {
    //     logger.Info(L"Random value: {0}", Math::Rand());
    // }

    for(int32 i = 0; i < 10; ++i)
    {
        auto uuid = UUID();
        logger.Info(L"UUID{0}: {1}", i, uuid);
    }
   
}

void TestArraySort()
{
    Array<int> arr = {1, 98, 34, 25, 19, 34, 1, 98, 34, 77, 999, 27, 100, 6, 28, 1888, 89, 9, 130};
    for (int i = 5'000'000; i >= 0; --i)
    {
        arr.Add(i);
    }

    int64 startTime = Time::MilliTime();

    //AlgoInternal::BubbleSort(arr.GetData(), arr.Size(), &AlgoInternal::Less<decltype(arr.First())>);
    //AlgoInternal::SelectionSort(arr.GetData(), arr.Size(), Less<int>());
    //AlgoInternal::HeapSort(arr.GetData(), arr.Size(), Less<int>());
    //Algo::QuickSort(arr.GetData(), arr.Size());
    //Algo::IntroSort(arr.GetData(), arr.Size());
    //std::sort(arr.begin(), arr.end(), Less<int>());
    arr.Sort();

    for (SizeType i = 1; i < arr.Size(); ++i)
    {
        if(arr[i - 1] > arr[i])
        {
            logger.Error(L"Sort error at pos {0}, value is {1}", i, arr[i]);
        }
    }

    logger.Info(L"Total used milliseconds: {0}", (Time::MilliTime() - startTime));
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
    bool ok;
    ok = StringConvert::TryParseFloat(L"-.50", ret);
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

void TestTime()
{
    logger.Info(L"Now: {0}", Time::NanoTime() / 1000000000);
    std::this_thread::sleep_for(Time::Seconds(1));
    logger.Info(L"Now: {0}", Time::MilliTime() / 1000);
    logger.Info(L"Next: {0}", Time::MilliTime(Time::Now() + Time::Seconds(1)) / 1000);
}

void TestVariant()
{
    Variant var(100);
    int32 intValue = var.GetValue<int32>();
    var = String(L"ABCD");
    String strValue = var.GetValue<String>();
    logger.Info(L"String value : {0}", strValue);
}

void TestTemplate()
{
    int32 i = 100;
    const int32* ptr = &i;
    Hash::HashValue(ptr);
    const char8* cstr = "HHHH";
    Hash::HashValue(cstr);

    UUID uuid;
    Hash::HashValue(uuid);
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

    // TestMath();
    // TestArraySort();
    // TestHashMap();
    // TestTime();
    TestVariant();

    //TestTemplate();
   

    // std::thread thread1 = std::thread([](){
    //     Log log1 = Log(L"Thread1");
    //     log1.Info(L"Thread start.");

    //     log1.Info(L"Thread finish.");
    // });
    // thread1.join();



    system("pause");
    return 0;
}