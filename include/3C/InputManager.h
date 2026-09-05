#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>

#include <cstdint>

class InputManager {
public:
	[[nodiscard]] static InputManager& GetInstance();
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	void Update();

	[[nodiscard]] bool IsKeyPressed(SDL_Scancode key) const { return m_keyState && m_keyState[key]; }
	[[nodiscard]] bool IsMouseBtnPressed(std::int32_t mouseBtn) const;

	[[nodiscard]] float GetMouseDeltaX() const { return m_mouseDeltaX; }
	[[nodiscard]] float GetMouseDeltaY() const { return m_mouseDeltaY; }

private:
	InputManager();

	const bool* m_keyState = nullptr;
	SDL_MouseButtonFlags m_mouseButtons = 0;
	float m_mouseDeltaX = 0.0f;
	float m_mouseDeltaY = 0.0f;
};

#endif // INPUTMANAGER_H
