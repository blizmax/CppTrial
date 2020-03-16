#include "Test/TestCoreLib.h"

#include "Core/Math.h"
#include "Core/Array.h"
#include "Core/List.h"
#include "Core/HashSet.h"
#include "Core/HashMap.h"
#include "Core/SortedSet.h"
#include "Core/SortedMap.h"
#include "Core/PriorityQueue.h"
#include "Core/String.h"
#include "Core/Algorithm.h"
#include "Core/String/StringEncode.h"
#include "Core/String/StringConvert.h"
#include "Core/Logger.h"
#include "Core/Thread.h"
#include "Core/Time.h"
#include "Core/Template.h"
#include "Core/Variant.h"
#include "Core/Any.h"
#include "Core/Exception.h"
#include "Core/Delegate.h"

namespace Test
{
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

    for (SizeType i = 1; i < arr.Count(); ++i)
    {
        if (arr[i - 1] > arr[i])
        {
            CT_LOG(Error, CT_TEXT("Sort error at pos {0}, value is {1}"), i, arr[i]);
        }
    }

    CT_LOG(Info, CT_TEXT("Total used milliseconds: {0}"), (Time::MilliTime() - startTime));
}

void TestHashMap()
{
    HashMap<String, int32> map1;

    map1.Put(CT_TEXT("A"), 1);
    map1.Put(CT_TEXT("2"), 2);
}

void TestSortedMap()
{
    SortedMap<int32, int32> map1{
        {1, 1}, {100, 100}, {70, 70}};

    map1.Put(35, 35);
    map1.Put(35, 35);
    map1.Put(35, 35);
    map1.Put(35, 35);
    map1.Put(35, 35);
    map1.Put(35, 35);

    for (uint32 i = 1; i < 100; ++i)
    {
        int32 v = Math::RandInt(0, 65535);
        map1.Put(v, v);
    }

    for (uint32 i = 10000; i <= 65535; ++i)
    {
        if (map1.Contains(i))
        {
            map1.Remove(i);
        }
    }

    for (const auto &e : map1)
    {
        CT_LOG(Info, CT_TEXT("{0}"), e.Key());
    }
}

void TestPriorityQueue()
{
    PriorityQueue<int32> queue{120, 130};
    queue.Push(140);

    for (int32 i = 1; i < 100; ++i)
    {
        queue.Push(i);
    }

    while (!queue.IsEmpty())
    {
        CT_LOG(Info, CT_TEXT("first:{0}"), queue.First());
        queue.Pop();
    }
}

void TestStringEncode()
{
    String str1 = StringEncode::UTF8::FromChars("😊😡/(ㄒoㄒ)/~~🐷");
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
    Logger logger(CT_TEXT(""));
    logger.SetLevel(LogLevel::Info);
    logger.Info(CT_TEXT("IIIIII"));
    logger.Error(CT_TEXT("{}{0}and{1}"), String(L"String1"), sizeof(double));
}

void TestTime()
{
    CT_LOG(Info, CT_TEXT("Now: {0}"), Time::NanoTime() / 1000000000);
    std::this_thread::sleep_for(Time::Seconds(1));
    CT_LOG(Info, CT_TEXT("Now: {0}"), Time::MilliTime() / 1000);
    CT_LOG(Info, CT_TEXT("Next: {0}"), Time::MilliTime(Time::Now() + Time::Seconds(1)) / 1000);
}

void TestVariant()
{
    Variant var(100);
    int32 intValue = var.GetValue<int32>();
    var = String(CT_TEXT("ABCD"));
    String strValue = var.GetValue<String>();
    CT_LOG(Info, CT_TEXT("String value : {0}"), strValue);
}

void TestAny()
{
    Any any(100LL);
    int32 val = any.Cast<int32>();
    CT_LOG(Info, CT_TEXT("Cast int32 : {0}"), val);
    int32 &ref = any.RefCast<int32>();
    ref = 300;
    CT_LOG(Info, CT_TEXT("RefCast int64 : {0}"), any.RefCast<int64>());
}

void TestDelegate()
{
    class B
    {
    public:
        void Print() const
        {
            CT_LOG(Info, CT_TEXT("Call Print."));
        }
        void Test(int32 a, int32 b, int32 c)
        {
            CT_LOG(Info, CT_TEXT("Call Test. {0} {1} {2}"), a, b, c);
        }
    };

    B obj;

    Delegate<void(void)> delegate;
    delegate.On(&B::Print, &obj);
    auto handle = delegate.On(&B::Print, &obj);
    delegate.On(TestAny);
    delegate();

    handle.Off();

    delegate();
}

} // namespace Test
