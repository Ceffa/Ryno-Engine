#version 430

layout(location = 0) in vec3 in_Position;
layout(location = 1) in mat4 in_M;
layout(location = 2) in mat4 in_VP;


void main()
{
	gl_Position = in_VP * in_M * vec4(in_Position, 1.0);
}




