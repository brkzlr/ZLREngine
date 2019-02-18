#include <iostream>
#include <SDL2/SDL.h>
#include <glad.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaders.h>
#include <camera.h>

unsigned int loadTexture(char const * path);

int main(){
	//Settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	SDL_Window* mainWindow;
	SDL_GLContext glContext;

	//Set GL Context options
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create our SDL GL window and renderer
	mainWindow = SDL_CreateWindow("ZLR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(mainWindow == NULL){
		std::cerr << "Could not create a window! Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	else{
		glContext = SDL_GL_CreateContext(mainWindow);
	}

	//Load all OpenGL function pointers using glad
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)){
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Set SDL and OpenGL options
	SDL_SetWindowGrab(mainWindow, SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);
	glEnable(GL_DEPTH_TEST);

	//Load and create texture
	unsigned int diffMap = loadTexture("textures/container.png");
	unsigned int specMap = loadTexture("textures/container_spec.png");

	//Vertex data
	float vertices[] = {
		//Vertex coord       //Tex coord  //Normal coord
    	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
    	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };

	//Initialize Vertex objects
	unsigned int VBO, cubeVAO, lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	//Configure Vertex objects
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Texture Coord Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Normals Attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Light lamp objects
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	Shaders shader("shaders/cubeVertex", "shaders/cubeFragment");
	Shaders lightShader("shaders/lightVertex", "shaders/lightFragment");

	bool quit = false;
	SDL_Event ev;

	//Frame timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//Our FPS Camera
	Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
	bool firstLook = true;

	//Light
	glm::vec3 lightPos = glm::vec3(1.0f, 0.0f, 1.5f);
	bool flash = false;

	//Render loop
	while (!quit){
		while(SDL_PollEvent(&ev) != 0){
			switch (ev.type){
				case(SDL_QUIT):
					quit = true;
					break;
				case (SDL_KEYDOWN):
					if(ev.key.keysym.sym == SDLK_ESCAPE){
						quit = true;
					}
					break;
			}
		}

		//Calculate deltaTime for per-frame time logic
		float currentFrame = SDL_GetTicks()/1000.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Query our keyboard state
		const Uint8 *keyState = SDL_GetKeyboardState(NULL);
		//Query our mouse position
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		//Camera Movement
		if( keyState[SDL_SCANCODE_W] ){
			cam.MoveCamera(Camera::FORWARD, deltaTime);
		}
		if( keyState[SDL_SCANCODE_S] ){
			cam.MoveCamera(Camera::BACKWARD, deltaTime);
		}
		if( keyState[SDL_SCANCODE_A] ){
			cam.MoveCamera(Camera::LEFT, deltaTime);
		}
		if( keyState[SDL_SCANCODE_D] ){
			cam.MoveCamera(Camera::RIGHT, deltaTime);
		}
		if( keyState[SDL_SCANCODE_LSHIFT] ){
			cam.Sprint(true);
		}
		else{
			cam.Sprint(false);
		}
		if( keyState[SDL_SCANCODE_F]){
			flash = true;
		}
		if( keyState[SDL_SCANCODE_G]){
			flash = false;
		}

		//Camera Mouse Look
		float xPos = mouseX - SCR_HEIGHT/2.0f;
		float yPos = SCR_WIDTH/2.0f - mouseY;
		//Small workaround hack to stop having camera jump to a random position at game start. TO DO
		if (firstLook){
			xPos = 0;
			yPos = 0;
			firstLook = false;
		}
		cam.CameraLook(xPos, yPos);
		//Warp mouse to center after each look so we won't have constrained yaw rotation
		SDL_WarpMouseInWindow(mainWindow, SCR_HEIGHT/2.0f, SCR_WIDTH/2.0f);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Change lightPos over time
		lightPos.x = sin(SDL_GetTicks() / 1000.0f) * 2.0f;
		lightPos.z = cos(SDL_GetTicks() / 1000.0f) * 1.5f;

		//Activate all buffer objects and shaders.
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specMap);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shader.setInt("material.diffuse", 0);
		shader.setInt("material.specular", 1);
		shader.setFloat("material.shineVal", 32.0f);
		shader.setVec3("viewPos", cam.getPosition());

		//Directional Light
		shader.setVec3("sun.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setVec3("sun.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		shader.setVec3("sun.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setVec3("sun.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

		//Point Light
		shader.setVec3("pLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		shader.setVec3("pLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.setVec3("pLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("pLight.position", lightPos);
		shader.setFloat("pLight.constant", 1.0f);
		shader.setFloat("pLight.linear", 0.09f);
		shader.setFloat("pLight.quadratic", 0.032f);

		//Spot Light
		shader.setVec3("flashlight.position", cam.getPosition());
		shader.setVec3("flashlight.direction", cam.getCamForward());
		shader.setVec3("flashlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("flashlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("flashlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("flashlight.constant", 1.0f);
		shader.setFloat("flashlight.linear", 0.09f);
		shader.setFloat("flashlight.quadratic", 0.032f);
		shader.setFloat("flashlight.cutoffAngle", glm::cos(glm::radians(12.5f)));
		shader.setFloat("flashlight.outerCutoff", glm::cos(glm::radians(15.0f)));
		shader.setBool("isFLon", flash);

		//Rotate cube over time
		glm::mat4 modelMat(1.0f);
		modelMat = glm::rotate(modelMat, static_cast<float>(SDL_GetTicks()/1000.0f) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		shader.setMat4("modelMatrix", modelMat);

		glm::mat4 viewMat(1.0f);
		viewMat = cam.GetViewMatrix();
		shader.setMat4("viewMatrix", viewMat);

		glm::mat4 projectionMat(1.0f);
		projectionMat = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
		shader.setMat4("projectionMatrix", projectionMat);

		//Also draw lamp object
		lightShader.use();
		lightShader.setMat4("projectionMatrix", projectionMat);
		lightShader.setMat4("viewMatrix", viewMat);
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		lightShader.setMat4("modelMatrix", modelMat);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		SDL_GL_SwapWindow(mainWindow);
	}

	//De-allocate all resources then quit
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;
	SDL_Quit();
	return 0;
}

unsigned int loadTexture(char const *path){
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data){
		GLenum format;
		if(nrComponents == 1)
			format = GL_RED;
		else if(nrComponents == 3)
			format = GL_RGB;
		else if(nrComponents == 4)
			format = GL_RGBA;
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else{
		std::cerr << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
