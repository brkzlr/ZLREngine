#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL_scancode.h>

class InputManager {
public:
	static InputManager* GetInstance();
	InputManager(const InputManager&) = delete;
	void operator=(const InputManager&) = delete;

	void Update();

	bool IsKeyPressed(SDL_Scancode key) const { return m_KeyState && m_KeyState[key]; }
	bool IsMouseBtnPressed(int mouseBtn) const;

	int GetMouseX() const { return m_MouseX; }
	int GetMouseY() const { return m_MouseY; }

private:
	InputManager();
	static InputManager* ms_Instance;

	const unsigned char* m_KeyState = nullptr;
	unsigned int m_MouseButtons = 0;
	int m_MouseX = 0;
	int m_MouseY = 0;
};

#endif //INPUTMANAGER_H
