#version 430

in vec3 in_Position;

uniform mat4 no_trans_VP;
uniform uint background;
out vec3 coords_3d;
out vec3 color;


float split(uint color, int n){
	int index = n * 8;
	return bitfieldExtract(color, index, 8) / 255.0f;
}

void main()
{
	gl_Position = no_trans_VP *  vec4(in_Position, 1.0);
	coords_3d = vec3(in_Position.xy,-in_Position.z); // true because centered in 0
	color = vec3(split(background, 0), split(background, 1), split(background, 2));
}
