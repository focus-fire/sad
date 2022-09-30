#include "InputManager.h"

sad::InputManager sad::InputManager::s_Instance;

sad::InputManager& sad::InputManager::GetInstance()
{
    return s_Instance;
}

sad::InputManager::~InputManager() {}

bool sad::InputManager::GetKey(int key)
{
    return false;
}

bool sad::InputManager::GetKeyDown(int key)
{
    return false;
}

bool sad::InputManager::GetKeyUp(int key)
{
    return false;
}

bool sad::InputManager::GetMouseButton(int button)
{
    return false;
}

bool sad::InputManager::GetMouseButtonDown(int button)
{
    return false;
}

bool sad::InputManager::GetMouseButtonUp(int button)
{
    return false;
}


