#pragma once

#include "Application/.Package.h"

class Clipboard
{
public:
    virtual void SetString(const String &value) = 0;
    virtual String GetString() = 0;

    static void Init();
};

extern Clipboard *gClipboard;
