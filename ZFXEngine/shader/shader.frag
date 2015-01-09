#version 330 core

in vec2 uv_pos;

out vec3 color;

uniform sampler2D tex_sample0;

void main()
{
	color = texture(tex_sample0, uv_pos).rgb;
	//color = vec3(0, 0.5, 0.5);
}