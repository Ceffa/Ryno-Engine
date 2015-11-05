#version 430

in vec3 in_Position;

uniform mat4 no_trans_VP;

out vec3 coords_3d;

void main()
{
	gl_Position = no_trans_VP *  vec4(in_Position, 1.0);
	coords_3d = vec3(in_Position.xy,-in_Position.z); // true because centered in 0
}