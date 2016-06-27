#version 430

in vec3 coords_3d;
in vec3 color;
out vec3 frag_color;

uniform samplerCube cube_map;
uniform uint have_skybox;

void main(){
	frag_color = have_skybox == 1 ? texture(cube_map, coords_3d).rgb : color;
}
