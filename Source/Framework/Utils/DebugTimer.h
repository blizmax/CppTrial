#pragma once

#include "Core/Timer.h"
#include "Utils/.Package.h"

class DebugTimer : public Timer
{
public:
    DebugTimer(const String &name)
        : Timer(name, [](Timer &t) {
              CT_LOG(Info, CT_TEXT("{0} total cost ms: {1}."), t.GetName(), t.GetElapsedMs());
          })
    {
    }
};