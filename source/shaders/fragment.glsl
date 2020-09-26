#version 440 core

in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D mainTexture;

void main()
{
	//fs_color = vec4(vs_color, 0.5f);
	fs_color = texture(mainTexture, vs_texcoord);
}