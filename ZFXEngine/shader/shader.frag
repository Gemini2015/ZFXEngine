#version 330 core

in vec2 uv_pos;
in vec3 LightAD;
in vec3 LightS;

out vec3 color;

uniform sampler2D tex_sample0;

void main()
{
	// //vec4 uvcolor = vec4(texture(tex_sample0, uv_pos).rgb, 1);
	 vec3 uvcolor = texture(tex_sample0, uv_pos).rgb;
	// //color = uvcolor;
	// color = min(uvcolor + 0.2 * LightColor, vec3(1.0));

	color = LightAD * uvcolor + LightS;
}