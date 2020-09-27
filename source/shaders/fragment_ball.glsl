#version 440 core

in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D mainTexture;

void main()
{
	fs_color = texture(mainTexture, vs_texcoord);
}