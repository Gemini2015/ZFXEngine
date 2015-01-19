#version 330 core

layout(location = 0) in vec4 vertex_postion;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 uvcoord;

out vec2 uv_pos;
out vec3 LightD;
out vec3 LightA;
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

uniform vec4 camera_position;

void main()
{
	// Vertex Position
	gl_Position = modelviewproj_matrix * vertex_postion;
	uv_pos = uvcoord;

	// Normal Matrix mat4 -> mat3
	mat3 normal_mat3 = mat3x3(normal_matrix);

	vec3 eyeNorm = normalize(normal_mat3 * vertex_normal);
	vec4 ModelViewVertexPosition = modelview_matrix * vertex_postion;

	vec3 s = normalize(vec3(light_position - ModelViewVertexPosition));
	float diffcos = dot(s, eyeNorm);
	LightD = light_diffuse.rgb * material_diffuse.rgb * diffcos;
	LightA = light_ambient.rbg * material_ambient.rgb;

	vec3 specularReflect = reflect(-s, eyeNorm);

	vec4 camera_position_view = modelview_matrix * camera_position;
	vec3 viewDir = normalize((camera_position_view - ModelViewVertexPosition).xyz);

	LightS = (light_specular.rgb * material_specular.rgb);
	LightS = LightS * pow(max(dot(specularReflect, viewDir), 0), material_shininess);
}