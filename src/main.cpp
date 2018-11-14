#include <iostream>
#include <SDL2/SDL.h>
#include <glad.h>
#include <shaders.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	unsigned int cubeTexture;
	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	//Set texture wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* tData = stbi_load("textures/brick.jpg", &width, &height, &nrChannels, 0);
	if (tData){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(tData);

	//Vertex data
	float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

	unsigned int indices[] = {
		0, 1, 3,//first triangle in cube
		1, 2, 3 //second triangle in cube
	};

	//Initialize Vertex objects
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Configure Vertex objects
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Texture Coord Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	Shaders shader("shaders/vertex", "shaders/fragment");

	bool quit = false;
	SDL_Event ev;

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
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activate all buffer objects and shader then render.
		shader.use();
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		SDL_GL_SwapWindow(mainWindow);

		//Rotate cube over time
		glm::mat4 model(1.0f);
		model = glm::rotate(model, static_cast<float>(SDL_GetTicks()/1000.0f) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		shader.setMat4("view", view);

		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
		shader.setMat4("projection", projection);
	}

	//De-allocate all resources then quit
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;
	SDL_Quit();
	return 0;
}