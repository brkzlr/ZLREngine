#include "3C/CameraManager.h"

#include "3C/InputManager.h"

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

#include <algorithm>
#include <cmath>

constexpr float cMaxPitch = 89.0f;

CameraManager& CameraManager::GetInstance()
{
	static CameraManager instance;
	return instance;
}

void CameraManager::Initialize(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	m_position = position;
	m_worldUp = up;
	m_yaw = yaw;
	m_pitch = std::clamp(pitch, -cMaxPitch, cMaxPitch);
	UpdateCameraVectors();
}

void CameraManager::Update(float deltaTime)
{
	const InputManager& inputMgr = InputManager::GetInstance();

	// Keyboard
	if (inputMgr.IsKeyPressed(SDL_SCANCODE_W)) {
		MoveCamera(ECameraDirection::FORWARD, deltaTime);
	}
	if (inputMgr.IsKeyPressed(SDL_SCANCODE_S)) {
		MoveCamera(ECameraDirection::BACKWARD, deltaTime);
	}
	if (inputMgr.IsKeyPressed(SDL_SCANCODE_A)) {
		MoveCamera(ECameraDirection::LEFT, deltaTime);
	}
	if (inputMgr.IsKeyPressed(SDL_SCANCODE_D)) {
		MoveCamera(ECameraDirection::RIGHT, deltaTime);
	}
	if (inputMgr.IsKeyPressed(SDL_SCANCODE_LSHIFT)) {
		m_isSprinting = true;
	}
	else {
		m_isSprinting = false;
	}

	// Mouse
	CameraLook(inputMgr.GetMouseDeltaX(), -inputMgr.GetMouseDeltaY());
}

void CameraManager::UpdateCameraVectors()
{
	const float cYawRad = glm::radians(m_yaw);
	const float cPitchRad = glm::radians(m_pitch);
	const float cCosPitch = std::cos(cPitchRad);

	glm::vec3 front;
	front.x = std::cos(cYawRad) * cCosPitch;
	front.y = std::sin(cPitchRad);
	front.z = std::sin(cYawRad) * cCosPitch;
	m_front = glm::normalize(front);

	// Re-calculate Right and Up vectors
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void CameraManager::MoveCamera(ECameraDirection direction, float deltaTime)
{
	float movementSpeed = m_movementSpeed;
	if (m_isSprinting) {
		movementSpeed *= 2.0f;
	}
	float camVelocity = movementSpeed * deltaTime;

	switch (direction) {
	case ECameraDirection::FORWARD:
		m_position += m_front * camVelocity;
		break;
	case ECameraDirection::BACKWARD:
		m_position -= m_front * camVelocity;
		break;
	case ECameraDirection::LEFT:
		m_position -= m_right * camVelocity;
		break;
	case ECameraDirection::RIGHT:
		m_position += m_right * camVelocity;
		break;
	}
}

void CameraManager::CameraLook(float xPos, float yPos)
{
	xPos *= m_mouseSens;
	yPos *= m_mouseSens;

	m_yaw += xPos;

	// Constrain the pitch so we don't run into weird physics past a certain point
	m_pitch = std::clamp(m_pitch + yPos, -cMaxPitch, cMaxPitch);

	UpdateCameraVectors();
}
