#include "Core/Math.h"
#include "Core/Array.h"
#include "Core/List.h"
#include "Core/HashSet.h"
#include "Core/HashMap.h"
#include "Core/SortedMap.h"
#include "Core/String.h"
#include "Core/Algorithm.h"
#include "Core/String/StringEncode.h"
#include "Core/String/StringConvert.h"
#include "Core/Log.h"
#include "Core/Thread.h"
#include "Core/Time.h"
#include "Core/Template.h"
#include "Core/Variant.h"
#include "Core/Any.h"
#include "Core/Exception.h"
#include "Core/Delegate.h"
#include "Utils/UUID.h"
#include "Utils/Name.h"
#include "Reflection/Test.h"
#include "IO/Test.h"
#include "Math/Test.h"

static Log logger = Log(L"Main");

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

void TestName()
{
    auto runnable = [](){
        for(uint32 i = 0; i < 1000; ++i)
        {
            Name name(StringConvert::ToString(i));
            //logger.Info(L"addr : {0}", data);
        }
    };

    Array<std::thread> threads;

    for(uint32 i = 0; i < 5; ++i)
    {
        threads.Add(std::thread(runnable));
    }

    for(auto& t : threads)
    {
        t.join();
    }

#ifdef CT_DEBUG
    auto nameDatas = Name::DebugDumpNameMap();
    for(const auto& data: nameDatas)
    {
        logger.Info(L"Name :{0}", data->string);
    }

    logger.Info(L"Count : {0}", nameDatas.Size());
#endif
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
    HashMap<String, int32> map1;
    
    map1.Put(CT_TEXT("A"), 1);
    map1.Put(CT_TEXT("2"), 2);

    // for(const auto&[key, value] : map1)
    // {

    // }
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

void TestAny()
{
    Any any(100LL);
    int32 val = any.Cast<int32>();
    logger.Info(L"Cast int32 : {0}", val);
    int32& ref = any.RefCast<int32>();
    ref = 300;
    logger.Info(L"RefCast int64 : {0}", any.RefCast<int64>());
}

void TestException()
{
    // Exception e(CT_TEXT("Temp"), CT_TEXT("Test Exception"), CT_EXCEPT_INNER(__FILE__), __LINE__);
    // logger.Info(CT_TEXT("Exception desc: {0}"), e);
    CT_EXCEPT(CT_TEXT("Temp"), CT_TEXT("Test Exception"));
}

void TestDelegate()
{
    class B
    {
        public:
        void Print() const
        {
            logger.Info(CT_TEXT("Call Print."));
        }
        void Test(int32 a, int32 b, int32 c)
        {
            logger.Info(CT_TEXT("Call Test. {0} {1} {2}"), a, b, c);
        }
    };

    B obj;

    Delegate<void(void)> delegate;
    delegate.Bind(&B::Print, &obj);
    delegate.Bind(&B::Print, &obj);
    delegate.Bind(TestAny);
    delegate();
    delegate.Unbind(&obj);
    delegate();

    Delegate<void(int32, int32, int32)> delegate1;
    delegate1.Bind(&B::Test, &obj);
    delegate1(100, 200, 300);

    auto f = &B::Print;
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
    // TestVariant();
    //TestAny();

    //TestException();
    //TestDelegate();

    //TestName();

    //TestTemplate();

    //Reflection::Test();

    // IO::Test();

    Test::TestMath();

    // std::thread thread1 = std::thread([](){
    //     Log log1 = Log(L"Thread1");
    //     log1.Info(L"Thread start.");

    //     log1.Info(L"Thread finish.");
    // });
    // thread1.join();



    system("pause");
    return 0;
}