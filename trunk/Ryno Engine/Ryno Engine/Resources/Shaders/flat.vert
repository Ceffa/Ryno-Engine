#version 430

in vec3 in_Position;

uniform vec3 Color;
uniform mat4 MVP;

out vec3 middle_color;

void main()
{
	gl_Position = MVP *  vec4(in_Position, 1.0);
	middle_color = Color;
}