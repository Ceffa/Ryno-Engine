#version 430

layout(location = 0) in vec3 in_Position;
layout(location = 4) in mat4 in_M;

uniform mat4 light_VP;

void main(){
	vec4 pos = vec4(in_Position, 1);
	gl_Position = light_VP * in_M * pos;
	

	
}
