#include "3C/InputManager.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

InputManager::InputManager()
{
	m_keyState = SDL_GetKeyboardState(nullptr);
}

void InputManager::Update()
{
	SDL_PumpEvents();
	m_mouseButtons = SDL_GetRelativeMouseState(&m_mouseDeltaX, &m_mouseDeltaY);
}

bool InputManager::IsMouseBtnPressed(std::int32_t mouseBtn) const
{
	return m_mouseButtons & SDL_BUTTON_MASK(mouseBtn);
}
