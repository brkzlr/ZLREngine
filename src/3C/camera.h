#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	enum Camera_Direction{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSens;
	bool bSprint;

	void updateCameraVectors();

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
	glm::mat4 GetViewMatrix();
	void MoveCamera(Camera_Direction direction, float deltaTime);
	void CameraLook(float xPos, float yPos);
	void Sprint(bool value);
	glm::vec3 getPosition() const;
	glm::vec3 getCamForward() const;
};

#endif