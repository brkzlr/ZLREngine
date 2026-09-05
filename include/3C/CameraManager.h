#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class CameraManager {
public:
	[[nodiscard]] static CameraManager& GetInstance();
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

	void Initialize(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	void Update(float deltaTime);

	// Getters
	[[nodiscard]] glm::vec3 GetPosition() const { return m_position; }
	[[nodiscard]] glm::vec3 GetCamForward() const { return m_front; }
	[[nodiscard]] glm::mat4 GetViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

private:
	CameraManager() = default;

	enum class ECameraDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	void UpdateCameraVectors();
	void MoveCamera(ECameraDirection direction, float deltaTime);
	void CameraLook(float xPos, float yPos);

	glm::vec3 m_position { 0.0f };
	glm::vec3 m_front { 0.0f };
	glm::vec3 m_up { 0.0f };
	glm::vec3 m_right { 0.0f };
	glm::vec3 m_worldUp { 0.0f };

	float m_yaw = 0.0f;
	float m_pitch = 0.0f;

	float m_movementSpeed = 2.5f; // Test value for now
	float m_mouseSens = 0.2f; // Test value for now
	bool m_isSprinting = false; // Move to entity class when created
};

#endif // CAMERAMANAGER_H
