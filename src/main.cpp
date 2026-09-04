#include "3C/CameraManager.h"
#include "3C/InputManager.h"
#include "Constants.h"
#include "ShaderProgram.h"

#include "External/glad.h"
#include "External/stb_image.h"
#include <SDL3/SDL.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <expected>
#include <iostream>
#include <utility>

unsigned int LoadTexture(const char* path);

int main()
{
	SDL_Window* mainWindow;
	SDL_GLContext glContext;

	// Set GL Context options
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Could not initialize SDL! Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create our SDL GL window and renderer
	mainWindow = SDL_CreateWindow("ZLR", SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
	if (mainWindow == nullptr) {
		std::cerr << "Could not create a window! Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	glContext = SDL_GL_CreateContext(mainWindow);
	if (glContext == nullptr) {
		std::cerr << "Could not create a 4.6 core GL context! Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Load all OpenGL function pointers using glad
	if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress))) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set SDL and OpenGL options
	if (!SDL_SetWindowRelativeMouseMode(mainWindow, true)) {
		std::cerr << "Could not enable relative mouse mode! Error: " << SDL_GetError() << std::endl;
	}
	glEnable(GL_DEPTH_TEST);

	int framebufferWidth = 0;
	int framebufferHeight = 0;
	SDL_GetWindowSizeInPixels(mainWindow, &framebufferWidth, &framebufferHeight);
	glViewport(0, 0, framebufferWidth, framebufferHeight);

	float aspectRatio = static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);

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

	{
		std::expected<ShaderProgram, std::string> shaderResult = ShaderProgram::Create("shaders/cubeVertex", "shaders/cubeFragment");
		if (!shaderResult) {
			std::cerr << shaderResult.error() << std::endl;
			return -1;
		}
		ShaderProgram shader = std::move(*shaderResult);

		std::expected<ShaderProgram, std::string> lightShaderResult = ShaderProgram::Create("shaders/lightVertex", "shaders/lightFragment");
		if (!lightShaderResult) {
			std::cerr << lightShaderResult.error() << std::endl;
			return -1;
		}
		ShaderProgram lightShader = std::move(*lightShaderResult);

		// Frame timing
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		// Our FPS Camera
		CameraManager::GetInstance()->Initialize(glm::vec3(0.0f, 0.0f, 3.0f), { 0.f, 1.f, 0.f }, -90.0f, 0.0f);

		// Light
		glm::vec3 lightPos(1.0f, 0.0f, 1.5f);
		bool flash = false;

		// Render loop
		bool quit = false;
		while (!quit) {
			// Calculate deltaTime for per-frame time logic
			float currentFrame = static_cast<float>(SDL_GetTicks()) / 1000.0f;
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
			projectionMat = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
			shader.SetMat4("projectionMatrix", projectionMat);
			glDrawArrays(GL_TRIANGLES, 0, 36);

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
	}

	// De-allocate all resources then quit
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;
	SDL_Quit();
	return 0;
}

unsigned int LoadTexture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data == nullptr) {
		std::cerr << "Texture failed to load at path: " << path << std::endl;
		return 0;
	}

	GLenum format;
	switch (nrComponents) {
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		std::cerr << "Texture has an unsupported component count of " << nrComponents << " at path: " << path << std::endl;
		stbi_image_free(data);
		return 0;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return textureID;
}
