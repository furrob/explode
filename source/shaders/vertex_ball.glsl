#version 440 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_normal;

out vec2 vs_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_texcoord = vec2(vertex_texcoord.x, -vertex_texcoord.y);

	gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
}