#version 440 core

in vec3 vCol;
in vec2 vTex;

out vec4 fCol;

uniform sampler2D texture0;

void main()
{
	fCol = vec4(vCol, 1.f);
	//fCol = mix(texture(texture0, vTex), vec4(vCol, 1.f), 0.5f);
}