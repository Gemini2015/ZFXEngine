#version 330 core

layout(location = 0) in vec4 vertex_postion;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 uvcoord;

out vec2 uv_pos;
out vec3 LightAD;
out vec3 LightS;

// Matrix
uniform mat4 modelview_matrix;
uniform mat4 modelviewproj_matrix;
uniform mat4 normal_matrix;

// Light
uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
//uniform vec3 light_Intensity;

// Material
uniform vec4 material_diffuse;
uniform vec4 material_ambient;
uniform vec4 material_specular;
uniform float material_shininess;



void main()
{
	// Vertex Position
	gl_Position = modelviewproj_matrix * vertex_postion;
	uv_pos = uvcoord;

	// Normal Matrix mat4 -> mat3
	mat3 normal_mat3 = mat3x3(normal_matrix);

	vec3 eyeNorm = normalize(normal_mat3 * vertex_normal);
	vec4 eyePosition = modelview_matrix * vertex_postion;

	vec3 s;
	if( light_position.w == 0.0)
		s = normalize(vec3(light_position));
	else
		s = normalize(vec3(light_position - eyePosition));

	vec3 v = normalize(vec3(-eyePosition));
	vec3 r = reflect(-s, eyeNorm);

	//vec3 light_Intensity = vec3(0.8, 0.2, 0.2);

	LightAD = light_ambient.rgb * material_ambient.rgb +
		light_diffuse.rgb * material_diffuse.rgb * max(dot(s, eyeNorm), 0.0);
	LightS = light_specular.rgb * material_specular.rgb * pow(max(dot(r, v), 0.0), material_shininess);
}