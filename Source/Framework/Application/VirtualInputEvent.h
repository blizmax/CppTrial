#pragma once

#include "Application/InputEvent.h"

class VirtualInputEvent : public Event
{
};

class AxisEvent : public VirtualInputEvent
{
public:
};

class ActionEvent : public VirtualInputEvent
{
public:
};