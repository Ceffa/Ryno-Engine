E(Includes/global)
E(Includes/point_include)


uniform samplerCubeShadow shadow_cube;
uniform float shadow_strength;
uniform int index;


layout(std430, binding = 1) buffer point_ssbo
{
	PointLight lights[];
};


out vec3 fracolor;


float get_shadow(vec3 light_world_dir, float fov) {
	
	float current_depth = vector_to_depth(light_world_dir, 1.0, fov);
	float bias = 0.0005;
	return min(1, (1 - shadow_strength) + texture(shadow_cube, vec4(light_world_dir, current_depth - bias)));
}




void main(){

	fracolor = get_shaded_fragment(lights[index], gl_FragCoord.xy);
}


