#version 330 core

layout(location = 0) in vec4 vertex_postion;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 uvcoord;

out vec2 uv_pos;

out vec3 PSNormal;
out vec4 ViewPosition;
out vec4 LightDirection;

uniform mat4 modelviewproj_matrix;
uniform mat4 normal_matrix;
uniform mat4 modelview_matrix;
uniform vec4 light_direction;

void main()
{
	// Vertex Position
	gl_Position = modelviewproj_matrix * vertex_postion;
	uv_pos = uvcoord;

	mat3 normal_matrix_mat3 = mat3x3(normal_matrix);

	PSNormal = normal_matrix_mat3 * vertex_normal;

	ViewPosition = modelview_matrix * vertex_postion;
	LightDirection = modelview_matrix * light_direction;
}