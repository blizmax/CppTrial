#include "Application/InputManager.h"

void InputManager::OnLoad()
{
}

void InputManager::OnUnload()
{
}

void InputManager::OnUpdate()
{
}

void InputManager::ProcessKeyTyped(int32 character)
{
    Input &input = GetInput();
    input.ProcessKeyTyped(character);
}

void InputManager::ProcessKeyDown(int32 key)
{
    Input &input = GetInput();
    key = TranslateKey(key);
    input.ProcessKeyDown(key);
}

void InputManager::ProcessKeyUp(int32 key)
{
    Input &input = GetInput();
    key = TranslateKey(key);
    input.ProcessKeyUp(key);
}

void InputManager::ProcessTouchDown(int32 button)
{
    Input &input = GetInput();
    input.ProcessTouchDown(button);
}

void InputManager::ProcessTouchUp(int32 button)
{
    Input &input = GetInput();
    input.ProcessTouchUp(button);
}

void InputManager::ProcessMouseMoved(int32 posX, int32 posY)
{
    Input &input = GetInput();
    input.ProcessMouseMoved(posX, posY);
}

void InputManager::ProcessMouseScrolled(int32 amount)
{
    Input &input = GetInput();
    input.ProcessMouseScrolled(amount);
}