#include "GLDebug.h"

#include "External/glad.h"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <print>
#include <string_view>

static std::string_view DebugSourceName(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "Window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "Shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "Third party";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "Application";
	case GL_DEBUG_SOURCE_OTHER:
		return "Other";
	default:
		return "Unknown source";
	}
}

static std::string_view DebugTypeName(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return "Error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "Deprecated behaviour";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "Undefined behaviour";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "Portability";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "Performance";
	case GL_DEBUG_TYPE_MARKER:
		return "Marker";
	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "Push group";
	case GL_DEBUG_TYPE_POP_GROUP:
		return "Pop group";
	case GL_DEBUG_TYPE_OTHER:
		return "Other";
	default:
		return "Unknown type";
	}
}

static std::string_view DebugSeverityName(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return "High";
	case GL_DEBUG_SEVERITY_MEDIUM:
		return "Medium";
	case GL_DEBUG_SEVERITY_LOW:
		return "Low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "Notification";
	default:
		return "Unknown severity";
	}
}

static void GLAD_API_PTR DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void*)
{
	std::println(std::cerr, "GL {} {} from {}, id {}: {}", DebugSeverityName(severity), DebugTypeName(type), DebugSourceName(source), id, std::string_view(message, static_cast<std::size_t>(length)));
}

bool EnableGLDebugOutput()
{
	GLint contextFlags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);
	if ((contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) {
		return false;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(DebugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	return true;
}
