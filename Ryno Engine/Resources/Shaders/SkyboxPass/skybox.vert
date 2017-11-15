#version 430

in vec3 in_Position;

uniform uint background;
out vec3 coords_3d;
out vec3 color;


layout(std140) uniform glob_ubo {
	mat4 V;
	mat4 iV;
	mat4 P;
	mat4 iP;
	mat4 VP;
	mat4 iVP;
	vec4 cameraPos;
	float time;
	int screen_width;
	int screen_height;
};


float split(uint color, int n){
	int index = n * 8;
	return bitfieldExtract(color, index, 8) / 255.0f;
}

void main()
{
	gl_Position = P * vec4((V *  vec4(in_Position, 0.0)).xyz,1.0);
	coords_3d = vec3(in_Position.xy,-in_Position.z); // true because centered in 0
	color = vec3(split(background, 0), split(background, 1), split(background, 2));
}
