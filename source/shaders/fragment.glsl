#version 440 core

in vec2 vs_texcoord;
in vec3 frag_pos;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D mainTexture;
uniform bool player_paddle;
uniform bool walls;
uniform bool ball;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 view_pos;

void main()
{
	//ambient lighting
	float ambientStrength = 0.15f;
	//specular lighting
	float specularStrength = 0.3f;


	vec3 temp_col;
	//store texture color without alpha
	if(walls)//this only because wall dont have any texture
	{
		temp_col = vec3(0.2f, 0.75f, 1.0f);
	}
	else
	{
		vec4 tex_col = texture(mainTexture, vs_texcoord);
		temp_col = vec3(tex_col.r, tex_col.g, tex_col.b);
	}
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	if(ball) //prevents ball to be rendered as black ball due to fact that light source is inside it
	{
		ambient = vec3(1.0f);
		diffuse = vec3(0.0f);
		specular = vec3(0.0f);
	}
	else
	{
	//ambient
		ambient = ambientStrength * light_color;

	//diffuse
		vec3 normal = normalize(vs_normal);

		vec3 light_dir = normalize(light_pos - frag_pos);

		float diff = max(dot(normal, light_dir), 0.0f);
		diffuse = diff * light_color;

	//specular
		vec3 view_dir = normalize(-view_pos - frag_pos);
		vec3 reflect_dir = reflect(-light_dir, normal);
		
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 64);
		specular = specularStrength * spec * light_color;
	}
	

	vec3 result = (ambient + diffuse + specular) * temp_col;

	if(player_paddle)
	{
		fs_color = vec4(result, 0.8f);
	}
	else
	{
		fs_color = vec4(result, 1.0f);
	}
	
}