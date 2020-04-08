#include "Application/DebugManager.h"

#if CT_DEBUG

DebugManager debugManager;
DebugManager *gDebugManager = &debugManager;

#include "Application/Application.h"

#if defined(_WIN32)
#include <windows.h>
#endif

void DebugManager::Startup()
{
#if defined(_WIN32)
    auto &gLog = Logger::GetGlobalLogger();
    gLog.printHandler.Clear();
    gLog.printHandler.On([](LogLevel l, const String &s) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
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
        SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | color);
        std::wcout << s.CStr() << std::endl;
    });
#endif

    auto &gProfiler = Profiler::GetGlobalProfiler();
    // gProfiler.sessionBeginEventHandler.On([](const auto& data){
    //     CT_LOG(Debug, CT_TEXT("SessionBegin, name:{0}"), data.name);
    // });
    // gProfiler.sessionEndEventHandler.On([](const auto& data){
    //     CT_LOG(Debug, CT_TEXT("SessionEnd, name:{0}"), data.name);
    // });
    // gProfiler.scopeEndEventHandler.On([](const auto& data){
    //     CT_LOG(Debug, CT_TEXT("ScopeEnd, name:{0}, elapsed:{1}, frameID:{2}"), data.name, data.elapsedTime, gApp->GetTotalFrames());
    // });
}

void DebugManager::Shutdown()
{

}

void DebugManager::Tick()
{

}

#endif