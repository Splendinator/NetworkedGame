#version 430 core

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;
layout(location = 3) uniform mat4 normMat;


in vec3 pos;
in vec3 norm;

out vec3 o_norm;
out vec3 worldPos;

void main()
{
	vec4 wp = vec4(pos, 1.0) * model;

	gl_Position = wp * view * proj;

	worldPos = vec3(wp);
	o_norm = norm;
}
