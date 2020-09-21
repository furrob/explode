#version 440 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexCol;
layout (location = 2) in vec2 vertexTex;

out vec3 vCol;
out vec2 vTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = projection * view * model * vec4(vertexPos, 1.0f);
	gl_Position = vec4(vertexPos, 1.0f);
	vCol = vertexCol;
	vTex = vec2(vertexTex.x, -vertexTex.y);
}