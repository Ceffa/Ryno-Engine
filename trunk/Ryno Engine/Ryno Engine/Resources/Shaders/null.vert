#version 430

in vec3 in_Position;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP *  vec4(in_Position, 1.0);
}