#version 330 core

layout(location = 0) in vec4 vertex_postion;
layout(location = 3) in vec2 uvcoord;

out vec2 uv_pos;
uniform mat4 modelviewproj_matrix;

void main()
{

	gl_Position = modelviewproj_matrix * vertex_postion;
	

	uv_pos = uvcoord;
}