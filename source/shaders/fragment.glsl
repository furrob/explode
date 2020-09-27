#version 440 core

in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D mainTexture;
uniform bool player_paddle;
uniform bool walls;

void main()
{
	//fs_color = vec4(vs_color, 0.5f);
	fs_color = texture(mainTexture, vs_texcoord);
	if(player_paddle == true)
	{
		fs_color.a = 0.5f; //50% transparency
	}
	else if(walls == true)
	{
		fs_color = vec4(0.3f, 1.0f, 0.4f, 1.0f);
	}
	
}