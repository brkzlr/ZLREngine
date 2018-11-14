#include <shaders.h>

Shaders::Shaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath){
	//We retrieve the source code from the files
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::ifstream geometryFile;

	//Activate exception throwing from ifstream objects
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		//Include geometry shader if it's present.
		if (geometryPath != nullptr){
			geometryFile.open(geometryPath);
			std::stringstream geometryStream;
			geometryStream << geometryFile.rdbuf();
			geometryFile.close();
			geometryCode = geometryStream.str();
		}
	}
	catch (std::ifstream::failure e){
		std::cerr << "Error! Shader file not succesfully read!" << std::endl;
	}
	//Transform code strings into c string style for OpenGL
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	unsigned int vertex, fragment;

	//Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "Vertex");

	//Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "Fragment");

	//Geometry Shader if it exists
	unsigned int geometry;
	if (geometryPath != nullptr){
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "Geometry");
	}

	//Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if(geometryPath != nullptr){
		glAttachShader(ID, geometry);
	}
	glLinkProgram(ID);
	checkCompileErrors(ID, "Program");

	//Delete the shaders after linkage
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if(geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shaders::use(){
	glUseProgram(ID);
}

//Shader uniform setting functions
void Shaders::setBool(const std::string &name, bool value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
void Shaders::setInt(const std::string &name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::setFloat(const std::string &name, float value) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::setVec2(const std::string &name, const glm::vec2 &value) const{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setVec3(const std::string &name, const glm::vec3 &value) const{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setVec4(const std::string &name, const glm::vec4 &value) const{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setMat2(const std::string &name, const glm::mat2 &mat) const{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shaders::setMat3(const std::string &name, const glm::mat3 &mat) const{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shaders::setMat4(const std::string &name, const glm::mat4 &mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shaders::checkCompileErrors(GLuint shader, std::string type){
	GLint success;
	GLchar log[1024];
	if(type != "Program"){
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(shader, 1024, NULL, log);
			std::cerr << "Error! Shader compilation error of type: " << type << "\n" << log << std::endl;
		}
	}
	else{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(shader, 1024, NULL, log);
			std::cerr << "Error! Program linking error!\n" << log << std::endl;
		}
	}
}