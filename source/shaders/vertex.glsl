#version 440 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_normal;

out vec2 vs_texcoord;
out vec3 vs_normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_texcoord = vec2(vertex_texcoord.x, -vertex_texcoord.y); //flip texcoords in y axis

	gl_Position = projection * view * model * vec4(vertex_position, 1.0f);

	frag_pos = vec3(model * vec4(vertex_position, 1.0f)); //position in world coords
	
	vs_normal = mat3(model) * vertex_normal;
}