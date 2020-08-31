#include "Application/DebugManager.h"
#include "Application/Application.h"

DebugManager debugManager;
DebugManager *gDebugManager = &debugManager;

#if CT_PLATFORM_WIN32
#include <windows.h>
static HANDLE hStdOutput;
#endif

void DebugManager::Startup()
{
#if CT_PLATFORM_WIN32
    auto &gLog = Logger::GetGlobal();
    hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    gLog.printHandler.Clear();
    gLog.printHandler.On([](LogLevel l, const String &s) {
        WORD color = 0;
        switch (l)
        {
        case LogLevel::Debug:
            color = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case LogLevel::Info:
            color = FOREGROUND_GREEN;
            break;
        case LogLevel::Warning:
            color = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        case LogLevel::Error:
            color = FOREGROUND_RED;
            break;
        default:
            color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }
        SetConsoleTextAttribute(hStdOutput, FOREGROUND_INTENSITY | color);
        std::wcout << s.CStr() << std::endl;
    });
#endif

    auto &gProfiler = Profiler::GetGlobalProfiler();

    gProfiler.sessionEndEventHandler.On([this](const auto &data) {
        cpuProfileData.AddSession(data);
    });
}

void DebugManager::Shutdown()
{
}

void DebugManager::Tick()
{
    ProfileEntryCollection collection;
    for (auto &e : cpuProfileData.entries)
    {
        collection.AddEntry(e);
    }
    cpuProfileData.Clear();
    cpuProfileRootEntry = std::move(collection.root);
}

const ProfileEntry &DebugManager::GetCpuProfileRootEntry() const
{
    return cpuProfileRootEntry;
}