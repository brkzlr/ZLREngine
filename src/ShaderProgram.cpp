#include "ShaderProgram.h"

#include "External/glad.h"

#include <expected>
#include <fstream>
#include <span>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct StageRequest {
	GLenum type;
	const char* name;
	const char* path;
};

constexpr GLsizei cInfoLogSize = 1024;

[[nodiscard]] static std::expected<std::string, std::string> ReadShaderFile(const char* path)
{
	std::ifstream file(path);
	if (!file.is_open()) {
		return std::unexpected(std::string("Could not open shader file: ") + path);
	}

	std::stringstream stream;
	stream << file.rdbuf();
	if (file.bad()) {
		return std::unexpected(std::string("Could not read shader file: ") + path);
	}

	return stream.str();
}

static void DeleteShaders(std::span<const GLuint> shaders)
{
	for (GLuint shader : shaders) {
		glDeleteShader(shader);
	}
}

[[nodiscard]] static std::expected<GLuint, std::string> CompileShader(const StageRequest& request, const char* source)
{
	GLuint shader = glCreateShader(request.type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar log[cInfoLogSize];
		glGetShaderInfoLog(shader, cInfoLogSize, nullptr, log);
		glDeleteShader(shader);
		return std::unexpected(std::string("Shader compilation error of type: ") + request.name + "\n" + log);
	}

	return shader;
}

[[nodiscard]] static std::expected<std::vector<GLuint>, std::string> CompileStages(std::span<const StageRequest> requests)
{
	std::vector<GLuint> shaders;

	for (const StageRequest& request : requests) {
		if (request.path == nullptr) {
			continue;
		}

		std::expected<std::string, std::string> source = ReadShaderFile(request.path);
		if (!source) {
			DeleteShaders(shaders);
			return std::unexpected(source.error());
		}

		std::expected<GLuint, std::string> shader = CompileShader(request, source->c_str());
		if (!shader) {
			DeleteShaders(shaders);
			return std::unexpected(shader.error());
		}

		shaders.push_back(*shader);
	}

	return shaders;
}

[[nodiscard]] static std::expected<GLuint, std::string> LinkProgram(std::span<const GLuint> shaders)
{
	GLuint program = glCreateProgram();
	for (GLuint shader : shaders) {
		glAttachShader(program, shader);
	}
	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar log[cInfoLogSize];
		glGetProgramInfoLog(program, cInfoLogSize, nullptr, log);
		glDeleteProgram(program);
		return std::unexpected(std::string("Program linking error!\n") + log);
	}

	return program;
}

std::expected<ShaderProgram, std::string> ShaderProgram::Create(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	const StageRequest cRequests[] = {
		{ .type = GL_VERTEX_SHADER, .name = "Vertex", .path = vertexPath },
		{ .type = GL_FRAGMENT_SHADER, .name = "Fragment", .path = fragmentPath },
		{ .type = GL_GEOMETRY_SHADER, .name = "Geometry", .path = geometryPath }
	};

	std::expected<std::vector<GLuint>, std::string> shaders = CompileStages(cRequests);
	if (!shaders) {
		return std::unexpected(shaders.error());
	}

	std::expected<GLuint, std::string> program = LinkProgram(*shaders);
	DeleteShaders(*shaders);
	if (!program) {
		return std::unexpected(program.error());
	}

	return ShaderProgram(*program);
}

ShaderProgram::ShaderProgram(GLuint id)
    : m_id(id)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_id(std::exchange(other.m_id, 0))
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this != &other) {
		glDeleteProgram(m_id);
		m_id = std::exchange(other.m_id, 0);
	}
	return *this;
}

void ShaderProgram::Use()
{
	glUseProgram(m_id);
}

// Shader uniform setting functions
void ShaderProgram::SetBool(const char* name, bool value)
{
	glProgramUniform1i(m_id, glGetUniformLocation(m_id, name), static_cast<GLint>(value));
}
void ShaderProgram::SetInt(const char* name, GLint value)
{
	glProgramUniform1i(m_id, glGetUniformLocation(m_id, name), value);
}
void ShaderProgram::SetFloat(const char* name, float value)
{
	glProgramUniform1f(m_id, glGetUniformLocation(m_id, name), value);
}
void ShaderProgram::SetVec2(const char* name, const glm::vec2& value)
{
	glProgramUniform2fv(m_id, glGetUniformLocation(m_id, name), 1, &value[0]);
}
void ShaderProgram::SetVec3(const char* name, const glm::vec3& value)
{
	glProgramUniform3fv(m_id, glGetUniformLocation(m_id, name), 1, &value[0]);
}
void ShaderProgram::SetVec4(const char* name, const glm::vec4& value)
{
	glProgramUniform4fv(m_id, glGetUniformLocation(m_id, name), 1, &value[0]);
}
void ShaderProgram::SetMat2(const char* name, const glm::mat2& mat)
{
	glProgramUniformMatrix2fv(m_id, glGetUniformLocation(m_id, name), 1, GL_FALSE, &mat[0][0]);
}
void ShaderProgram::SetMat3(const char* name, const glm::mat3& mat)
{
	glProgramUniformMatrix3fv(m_id, glGetUniformLocation(m_id, name), 1, GL_FALSE, &mat[0][0]);
}
void ShaderProgram::SetMat4(const char* name, const glm::mat4& mat)
{
	glProgramUniformMatrix4fv(m_id, glGetUniformLocation(m_id, name), 1, GL_FALSE, &mat[0][0]);
}
