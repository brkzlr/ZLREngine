#include "3C/InputManager.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

InputManager* InputManager::ms_Instance = nullptr;

InputManager* InputManager::GetInstance()
{
	if (ms_Instance == nullptr) {
		ms_Instance = new InputManager();
	}
	return ms_Instance;
}

InputManager::InputManager()
{
	m_KeyState = SDL_GetKeyboardState(nullptr);
}

void InputManager::Update()
{
	SDL_PumpEvents();
	m_MouseButtons = SDL_GetRelativeMouseState(&m_MouseDeltaX, &m_MouseDeltaY);
}

bool InputManager::IsMouseBtnPressed(int mouseBtn) const
{
	return m_MouseButtons & SDL_BUTTON_MASK(mouseBtn);
}
