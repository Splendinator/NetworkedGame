#version 430 core

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;


in vec3 pos;
in vec3 norm;

void main()
{
	gl_Position = vec4(pos, 1.0) *view * proj;
}
