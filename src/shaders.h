#ifndef SHADERS_H
#define SHADERS_H

class Shaders {
private:
	unsigned int m_ID;
	void CheckCompileErrors(unsigned int shader, const std::string& type);

public:
	// We'll leave geometryPath as null as we do not use geometry shaders for now.
	Shaders(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Use();

	// Shader uniform setting functions
	void SetBool(const char* name, bool value);
	void SetInt(const char* name, int value);
	void SetFloat(const char* name, float value);
	void SetVec2(const char* name, const glm::vec2& value);
	void SetVec3(const char* name, const glm::vec3& value);
	void SetVec4(const char* name, const glm::vec4& value);
	void SetMat2(const char* name, const glm::mat2& mat);
	void SetMat3(const char* name, const glm::mat3& mat);
	void SetMat4(const char* name, const glm::mat4& mat);
};
#endif
