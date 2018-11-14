#ifndef SHADERS_H
#define SHADERS_H

#include <glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shaders{
private:
	unsigned int ID;
	void checkCompileErrors(GLuint shader, std::string type);

public:
	//We'll leave geometryPath as null as we do not use geometry shaders for now.
	Shaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();

	//Shader uniform setting functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
#endif