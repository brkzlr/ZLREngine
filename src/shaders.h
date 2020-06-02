#ifndef SHADERS_H
#define SHADERS_H

#include "External/glad.h"
#include <glm/glm.hpp>
#include <string>

class Shaders{
private:
	unsigned int m_ID;
	void CheckCompileErrors(GLuint shader, const std::string& type);

public:
	//We'll leave geometryPath as null as we do not use geometry shaders for now.
	Shaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Use();

	//Shader uniform setting functions
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVec2(const std::string& name, const glm::vec2& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec4(const std::string& name, const glm::vec4& value);
	void SetMat2(const std::string& name, const glm::mat2& mat);
	void SetMat3(const std::string& name, const glm::mat3& mat);
	void SetMat4(const std::string& name, const glm::mat4& mat);
};
#endif