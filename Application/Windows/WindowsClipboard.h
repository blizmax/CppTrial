#pragma once

#include "Application/Clipboard.h"

class WindowsClipboard : public Clipboard
{
    virtual void SetString(const String &value) override;
    virtual String GetString() override;
};