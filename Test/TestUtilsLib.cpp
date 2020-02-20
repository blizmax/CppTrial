#include "Test/TestUtilsLib.h"

#include "Core/Thread.h"

#include "Utils/UUID.h"
#include "Utils/Name.h"

namespace Test
{
void TestName()
{
    auto runnable = []() {
        for (uint32 i = 0; i < 1000; ++i)
        {
            Name name(StringConvert::ToString(i));
        }
    };

    Array<std::thread> threads;

    for (uint32 i = 0; i < 5; ++i)
    {
        threads.Add(std::thread(runnable));
    }

    for (auto &t : threads)
    {
        t.join();
    }

#ifdef CT_DEBUG
    auto nameDatas = Name::DebugDumpNameMap();
    for (const auto &data : nameDatas)
    {
        CT_LOG(Info, CT_TEXT("Name :{0}"), data->string);
    }

    CT_LOG(Info, CT_TEXT("Count : {0}"), nameDatas.Size());
#endif
}
}