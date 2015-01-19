#version 330 core

in vec2 uv_pos;
in vec3 LightA;
in vec3 LightD;
in vec3 LightS;

out vec3 color;

uniform sampler2D tex_sample0;

void main()
{
	vec3 uvcolor = texture(tex_sample0, uv_pos).rgb;
	color = (LightD + LightA) * uvcolor + LightS;
}