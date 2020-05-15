#pragma once

#include "Application/Clipboard.h"

class DesktopClipboard : public Clipboard
{
    virtual void SetString(const String &value) override;
    virtual String GetString() override;
};