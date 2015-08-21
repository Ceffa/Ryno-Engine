#version 430

layout(location = 0) in vec3 in_Position;
layout(location = 4) in mat4 in_M;

uniform mat4 light_VP;
//out vec3 world_pos;


void main()
{
	vec4 temp_world_pos = in_M * vec4(in_Position, 1.0);
	gl_Position = light_VP * temp_world_pos;
	//world_pos = temp_world_pos.xyz ;
}



