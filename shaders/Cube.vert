#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec3 a_normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main(){
	//Vertex Positioning and textures
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0f);
	v_texCoord = a_texCoord;

	//Lighting
	v_normal = mat3(transpose(inverse(u_modelMatrix))) * a_normal;
	v_fragPos = vec3(u_modelMatrix * vec4(a_position, 1.0));
}
