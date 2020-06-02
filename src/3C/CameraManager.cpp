#include "CameraManager.h"

CameraManager* CameraManager::ms_Instance = nullptr;

CameraManager* CameraManager::GetInstance(){
	if (ms_Instance == nullptr){
		ms_Instance = new CameraManager();
	}
	return ms_Instance;
}

void CameraManager::Initialize(glm::vec3 position, glm::vec3 up, float yaw, float pitch){
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

void CameraManager::Update(){
	//Check if movement input was made through events.
}

void CameraManager::UpdateCameraVectors(){
	glm::vec3 front(1.0f);
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	//Re-calculate Right and Up vectors
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}

void CameraManager::MoveCamera(ECameraDirection direction, float deltaTime){
	if (m_Sprint){
		m_MovementSpeed *= 2.0f;
	}
	float camVelocity = m_MovementSpeed * deltaTime;

	switch(direction){
		case ECameraDirection::FORWARD:
			m_Position += m_Front * camVelocity;
			break;
		case ECameraDirection::BACKWARD:
			m_Position -= m_Front * camVelocity;
			break;
		case ECameraDirection::LEFT:
			m_Position -= m_Right * camVelocity;
			break;
		case ECameraDirection::RIGHT:
			m_Position += m_Right * camVelocity;
			break;
	}
}

void CameraManager::CameraLook(float xPos, float yPos){
	xPos *= m_MouseSens;
	yPos *= m_MouseSens;

	m_Yaw += xPos;
	m_Pitch += yPos;

	//Constrain the pitch so we don't run into weird physics past a certain point
	if(m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if(m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	UpdateCameraVectors();
}