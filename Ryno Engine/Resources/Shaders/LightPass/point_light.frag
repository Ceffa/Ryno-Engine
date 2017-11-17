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

//This function generate a depth value from the direction vector, so that it can be compared 
//with the depth value in the shadow cube
float vector_to_depth(vec3 light_vec, float n, float f)
{
	vec3 AbsVec = abs(light_vec);
	float LocalZcomp = max(AbsVec.x, max(AbsVec.y, AbsVec.z));

	float NormZComp = (f + n) / (f - n) - (2 * f*n) / (f - n) / LocalZcomp;
	return (NormZComp + 1.0) * 0.5;
}

float get_shadow(vec3 light_world_dir, float fov) {
	
	float current_depth = vector_to_depth(light_world_dir, 1.0, fov);
	float bias = 0.0005;
	return lerp (1, texture(shadow_cube, vec4(light_world_dir, current_depth - bias)),shadow_strength);
}




void main(){

	fracolor = get_shaded_fragment(lights[index], ivec2(gl_FragCoord.xy));
}


