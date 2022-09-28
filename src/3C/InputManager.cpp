#include "precomp.h"
#include "InputManager.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>

InputManager* InputManager::ms_Instance = nullptr;

InputManager* InputManager::GetInstance(){
	if (ms_Instance == nullptr){
		ms_Instance = new InputManager();
	}
	return ms_Instance;
}

InputManager::InputManager(){
	m_KeyState = SDL_GetKeyboardState(nullptr);
}

void InputManager::Update(){
	SDL_PumpEvents();
	m_MouseButtons = SDL_GetMouseState(&m_MouseX, &m_MouseY);
}

bool InputManager::IsMouseBtnPressed(int mouseBtn) const {
	return m_MouseButtons & SDL_BUTTON(mouseBtn);
}

