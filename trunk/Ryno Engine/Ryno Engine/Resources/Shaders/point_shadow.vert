#version 430

layout(location = 0) in vec3 in_Position;
layout(location = 1) in mat4 in_M;

out vec3 pos;

void main()
{
	vec4 pos_temp = in_M * vec4(in_Position, 1.0);
	pos = pos_temp.xyz;
	gl_Position = pos_temp;
}



