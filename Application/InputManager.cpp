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

void InputManager::ProcessKeyTyped(char32 character)
{
    //CT_LOG(Warning, CT_TEXT("Key typed, char:{0}"), (int32)character);
}

void InputManager::ProcessKeyDown(int32 key)
{
    key = TranslateKey(key);
    //CT_LOG(Warning, CT_TEXT("Key down, key:{0}"), key);
}

void InputManager::ProcessKeyUp(int32 key)
{
    key = TranslateKey(key);
    //CT_LOG(Warning, CT_TEXT("Key up, key:{0}"), key);
}

void InputManager::ProcessTouchDown(int32 button)
{
    //CT_LOG(Warning, CT_TEXT("Touch down, button:{0}"), button);
}

void InputManager::ProcessTouchUp(int32 button)
{
    //CT_LOG(Warning, CT_TEXT("Touch up, key:{0}"), button);
}

void InputManager::ProcessMouseMoved(int32 posX, int32 posY)
{
    //CT_LOG(Warning, CT_TEXT("Mouse move, x:{0}, y:{1}"), posX, posY);
}

void InputManager::ProcessMouseScrolled(int32 amount)
{
    //CT_LOG(Warning, CT_TEXT("Mouse scroll, amount:{0}"), amount);
}