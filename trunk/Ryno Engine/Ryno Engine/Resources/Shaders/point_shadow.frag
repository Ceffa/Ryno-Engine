#version 430


in vec3 world_pos;		//Posizione nel mondo del fragment
uniform vec3 light_world_pos;	//Posizione nel mondo della luce

layout(location = 0) out float color;

void main() {
	
	vec3 distance_vector = world_pos - light_world_pos;
	float distance = length(distance_vector);
	color = distance/1000.0;
}
