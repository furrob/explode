#version 440 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_normal;

out vec3 vs_color;
out vec2 vs_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_color = vec3(1.0f, 0.5f, 0.75f);

	vs_texcoord = vertex_texcoord;

	gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
}