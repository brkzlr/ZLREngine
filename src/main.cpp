#include <SDL2/SDL.h>
#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include "External/glad.h"
#include "External/stb_image.h"

#include "3C/CameraManager.h"
#include "3C/InputManager.h"
#include "Constants.h"
#include "shaders.h"

unsigned int LoadTexture(const char* path);

int main()
{
	SDL_Window* mainWindow;
	SDL_GLContext glContext;

	// Set GL Context options
	SDL_InitSubSystem(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create our SDL GL window and renderer
	mainWindow = SDL_CreateWindow("ZLR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (mainWindow == nullptr) {
		std::cerr << "Could not create a window! Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	else {
		glContext = SDL_GL_CreateContext(mainWindow);
	}

	// Load all OpenGL function pointers using glad
	if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress))) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set SDL and OpenGL options
	SDL_SetWindowGrab(mainWindow, SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);
	glEnable(GL_DEPTH_TEST);

	// Load and create texture
	unsigned int diffMap = LoadTexture("textures/container.png");
	unsigned int specMap = LoadTexture("textures/container_spec.png");

	// Vertex data
	float vertices[] = {
		// Vertex coord     //Tex coord  //Normal coord
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};

	// Initialize Vertex objects
	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	// Configure Vertex objects
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture Coord Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Normals Attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Light lamp objects
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shaders shader("shaders/cubeVertex", "shaders/cubeFragment");
	Shaders lightShader("shaders/lightVertex", "shaders/lightFragment");

	// Frame timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Our FPS Camera
	CameraManager::GetInstance()->Initialize(glm::vec3(0.0f, 0.0f, 3.0f), { 0.f, 1.f, 0.f }, 180, -90);

	// Light
	glm::vec3 lightPos(1.0f, 0.0f, 1.5f);
	bool flash = false;

	// Render loop
	bool quit = false;
	while (!quit) {
		// Calculate deltaTime for per-frame time logic
		float currentFrame = SDL_GetTicks() / 1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		InputManager* inputMgr = InputManager::GetInstance();
		CameraManager* camMgr = CameraManager::GetInstance();
		inputMgr->Update();
		camMgr->Update(deltaTime);

		// Query our keyboard state
		if (inputMgr->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
			quit = true;
		}
		if (inputMgr->IsKeyPressed(SDL_SCANCODE_F)) {
			flash = true;
		}
		if (inputMgr->IsKeyPressed(SDL_SCANCODE_G)) {
			flash = false;
		}
		SDL_WarpMouseInWindow(mainWindow, SCR_HEIGHT / 2.0f, SCR_WIDTH / 2.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Change lightPos over time
		lightPos.x = sin(currentFrame) * 2.0f;
		lightPos.z = cos(currentFrame) * 1.5f;

		// Activate all buffer objects and shaders.
		shader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specMap);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shader.SetInt("material.diffuse", 0);
		shader.SetInt("material.specular", 1);
		shader.SetFloat("material.shineVal", 32.0f);
		shader.SetVec3("viewPos", camMgr->GetPosition());

		// Directional Light
		shader.SetVec3("sun.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.SetVec3("sun.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		shader.SetVec3("sun.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.SetVec3("sun.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

		// Point Light
		shader.SetVec3("pLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.SetVec3("pLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.SetVec3("pLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.SetVec3("pLight.position", lightPos);
		shader.SetFloat("pLight.constant", 1.0f);
		shader.SetFloat("pLight.linear", 0.09f);
		shader.SetFloat("pLight.quadratic", 0.032f);

		// Spot Light
		shader.SetVec3("flashlight.position", camMgr->GetPosition());
		shader.SetVec3("flashlight.direction", camMgr->GetCamForward());
		shader.SetVec3("flashlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.SetVec3("flashlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.SetVec3("flashlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.SetFloat("flashlight.constant", 1.0f);
		shader.SetFloat("flashlight.linear", 0.09f);
		shader.SetFloat("flashlight.quadratic", 0.032f);
		shader.SetFloat("flashlight.cutoffAngle", glm::cos(glm::radians(12.5f)));
		shader.SetFloat("flashlight.outerCutoff", glm::cos(glm::radians(15.0f)));
		shader.SetBool("isFLon", flash);

		// Rotate cube over time
		glm::mat4 modelMat(1.0f);
		modelMat = glm::rotate(modelMat, currentFrame * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		shader.SetMat4("modelMatrix", modelMat);

		glm::mat4 viewMat(1.0f);
		viewMat = camMgr->GetViewMatrix();
		shader.SetMat4("viewMatrix", viewMat);

		glm::mat4 projectionMat(1.0f);
		projectionMat = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
		shader.SetMat4("projectionMatrix", projectionMat);

		// Also draw lamp object
		lightShader.Use();
		lightShader.SetMat4("projectionMatrix", projectionMat);
		lightShader.SetMat4("viewMatrix", viewMat);
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		lightShader.SetMat4("modelMatrix", modelMat);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		SDL_GL_SwapWindow(mainWindow);
	}

	// De-allocate all resources then quit
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;
	SDL_Quit();
	return 0;
}

unsigned int LoadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		switch (nrComponents) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cerr << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
