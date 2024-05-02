#include "shaders.h"
#include "External/glad.h"
#include "precomp.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shaders::Shaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// We retrieve the source code from the files
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::ifstream geometryFile;

	// Activate exception throwing from ifstream objects
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		// Include geometry shader if it's present.
		if (geometryPath != nullptr) {
			geometryFile.open(geometryPath);
			std::stringstream geometryStream;
			geometryStream << geometryFile.rdbuf();
			geometryFile.close();
			geometryCode = geometryStream.str();
		}
	} catch (std::ifstream::failure e) {
		std::cerr << "Error! Shader file not succesfully read!" << std::endl;
	}
	// Transform code strings into c string style for OpenGL
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	unsigned int vertex, fragment;

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "Vertex");

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "Fragment");

	// Geometry Shader if it exists
	unsigned int geometry;
	if (geometryPath != nullptr) {
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, nullptr);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "Geometry");
	}

	// Shader Program
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	if (geometryPath != nullptr) {
		glAttachShader(m_ID, geometry);
	}
	glLinkProgram(m_ID);
	CheckCompileErrors(m_ID, "Program");

	// Delete the shaders after linkage
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr) {
		glDeleteShader(geometry);
	}
}

void Shaders::Use()
{
	glUseProgram(m_ID);
}

// Shader uniform setting functions
void Shaders::SetBool(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(m_ID, name), static_cast<int>(value));
}
void Shaders::SetInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(m_ID, name), value);
}
void Shaders::SetFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(m_ID, name), value);
}
void Shaders::SetVec2(const char* name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}
void Shaders::SetVec3(const char* name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}
void Shaders::SetVec4(const char* name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}
void Shaders::SetMat2(const char* name, const glm::mat2& mat)
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &mat[0][0]);
}
void Shaders::SetMat3(const char* name, const glm::mat3& mat)
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &mat[0][0]);
}
void Shaders::SetMat4(const char* name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &mat[0][0]);
}

void Shaders::CheckCompileErrors(unsigned int shader, const std::string& type)
{
	GLint success;
	GLchar log[1024];
	if (type != "Program") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, log);
			std::cerr << "Error! Shader compilation error of type: " << type << "\n"
			          << log << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, nullptr, log);
			std::cerr << "Error! Program linking error!\n"
			          << log << std::endl;
		}
	}
}
