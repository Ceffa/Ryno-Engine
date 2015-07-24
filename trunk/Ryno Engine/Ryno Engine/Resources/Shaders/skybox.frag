#version 330

in vec3 coords_3d;
out vec3 frag_color;

uniform samplerCube cube_map;

void main(){
	frag_color = texture(cube_map, coords_3d).rgb;
}