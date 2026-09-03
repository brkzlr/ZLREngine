#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>

class InputManager {
public:
	static InputManager* GetInstance();
	InputManager(const InputManager&) = delete;
	void operator=(const InputManager&) = delete;

	void Update();

	bool IsKeyPressed(SDL_Scancode key) const { return m_KeyState && m_KeyState[key]; }
	bool IsMouseBtnPressed(int mouseBtn) const;

	float GetMouseDeltaX() const { return m_MouseDeltaX; }
	float GetMouseDeltaY() const { return m_MouseDeltaY; }

private:
	InputManager();
	static InputManager* ms_Instance;

	const bool* m_KeyState = nullptr;
	SDL_MouseButtonFlags m_MouseButtons = 0;
	float m_MouseDeltaX = 0.0f;
	float m_MouseDeltaY = 0.0f;
};

#endif // INPUTMANAGER_H
