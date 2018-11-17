#include <camera.h>

void Camera::updateCameraVectors(){
	glm::vec3 front(1.0f);
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	//Re-calculate Right and Up vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up    = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch){
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	MovementSpeed = 2.5f;
	MouseSens = 0.2f;
	bSprint = false;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::MoveCamera(Camera_Direction direction, float deltaTime){
	float camVelocity = MovementSpeed * deltaTime;
	if (bSprint)
		camVelocity *= 2.0f;
	switch(direction){
		case FORWARD:
			Position += Front * camVelocity;
			break;
		case BACKWARD:
			Position -= Front * camVelocity;
			break;
		case LEFT:
			Position -= Right * camVelocity;
			break;
		case RIGHT:
			Position += Right * camVelocity;
			break;
	}
}

void Camera::CameraLook(float xPos, float yPos){
	xPos *= MouseSens;
	yPos *= MouseSens;

	Yaw += xPos;
	Pitch += yPos;

	//Constrain the pitch so we don't run into weird physics past a certain point
	if(Pitch > 89.0f)
		Pitch = 89.0f;
	if(Pitch < -89.0f)
		Pitch = -89.0f;

	updateCameraVectors();
}

void Camera::Sprint(bool value){
	bSprint = value;
}

glm::vec3 Camera::getPosition() const{
	return Position;
}