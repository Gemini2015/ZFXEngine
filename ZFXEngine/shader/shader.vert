#version 330 core

layout(location = 0) in vec4 vertex_postion;
layout(location = 3) in vec2 uvcoord;

out vec2 uv_pos;

uniform mat4 modelview;

uniform mat4 projection;

void main()
{
	gl_Position = projection * modelview * vertex_postion;

	uv_pos = uvcoord;
}