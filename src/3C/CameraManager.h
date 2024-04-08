#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

class CameraManager{
public:
	static CameraManager* GetInstance();
	CameraManager(const CameraManager&) = delete;
	void operator=(const CameraManager&) = delete;

	void Initialize(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	void Update(float deltaTime);

	//Getters
	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetCamForward() const { return m_Front; }
	glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

private:
	CameraManager() = default;
	static CameraManager* ms_Instance;

	enum class ECameraDirection{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	void UpdateCameraVectors();
	void MoveCamera(ECameraDirection direction, float deltaTime);
	void CameraLook(float xPos, float yPos);

	glm::vec3 m_Position{0.0f};
	glm::vec3 m_Front{0.0f};
	glm::vec3 m_Up{0.0f};
	glm::vec3 m_Right{0.0f};
	glm::vec3 m_WorldUp{0.0f};

	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;

	float m_MovementSpeed = 2.5f; //Test value for now
	float m_MouseSens = 0.2f;	  //Test value for now
	bool m_Sprint = false;      //Move to entity class when created
};

#endif
