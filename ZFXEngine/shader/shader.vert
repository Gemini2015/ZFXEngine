#version 330 core

layout(location = 0) in vec4 vertex_postion;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 uvcoord;

out vec2 uv_pos;
out vec3 normal;

out vec3 postion_worldspace;
out vec3 EyeDirection;
out vec3 lightDirection;

uniform mat4 modelview_matrix;
uniform mat4 modelviewproj_matrix;
uniform mat4 normal_matrix;
uniform vec4 light_position;

void main()
{

	gl_Position = modelviewproj_matrix * vertex_postion;
	
	mat3 normal_mat3 = mat3x3(normal_matrix);

	normal = normalize(normal_mat3 * vertex_normal);
	uv_pos = uvcoord;

	mat4 model_matrix = mat4(1.0);
	postion_worldspace = (model_matrix * vertex_postion).xyz;

	vec3 position_camera = (modelview_matrix * vertex_postion).xyz;
	EyeDirection = vec3(0, 0, 0) - position_camera;

	vec3 lightpostion_camera = (modelview_matrix * light_position).xyz;
	lightDirection = lightpostion_camera + EyeDirection;
}