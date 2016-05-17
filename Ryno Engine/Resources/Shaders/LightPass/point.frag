#version 430

//Structures

//4-byte aligned
struct PointLight{
	vec3 position; 
	float attenuation;
	uint diffuse;
	uint specular;
	float diffuse_intensity;
	float specular_intensity;
};

float split(uint color, int n);

//Unifroms taken by the buffers
//uniform sampler2D position_tex;
uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;
uniform sampler2D depth_tex;
uniform samplerCubeShadow shadow_cube;
uniform int shadows_enabled;

//Inverse matrix to rebuild position from depth
uniform mat4 inverse_P_matrix;
uniform mat4 inverse_VP_matrix;
uniform mat4 V_matrix;
//All the point light uniforms
uniform PointLight point_light;
//Screen size uniforms
uniform int screen_width;
uniform int screen_height;

//Max fov of the light, to reconstruct depth correctly
uniform float max_fov;

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

void main(){
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);
	
	//Rebuild position from depth
	float depth = texture(depth_tex, uv_coords).r *2.0-1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);
	vec4 position_view_space = inverse_P_matrix * position_screen_space;
	vec3 position = position_view_space.xyz / position_view_space.w;
	vec4 position_world_space = inverse_VP_matrix * position_screen_space;
	vec3 world_position = position_world_space.xyz / position_world_space.w;

	vec4 view_world_pos = V_matrix * vec4(point_light.position, 1);

	//Color directly from g buffer
	vec4 sample_diff = texture(diffuse_tex, uv_coords);
	vec3 mat_diff = sample_diff.rgb;
	float flatness = sample_diff.w;
	vec4 sample_spec = texture(specular_tex, uv_coords);
	vec3 mat_spec = sample_spec.rgb;
	float mat_spec_pow = sample_spec.w;
	
	//Normal z-axis built back from the other two
	vec2 n = texture(normal_tex, uv_coords).xy;
	vec3 normal = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));

	//Important vectors
	vec3 not_normal_ligth_dir = view_world_pos.xyz - position;
	vec3 light_dir = normalize(not_normal_ligth_dir);
	vec3 view_dir = normalize(-position);
	vec3 half_dir = normalize(light_dir + view_dir);

	//Calculate attenuation
	float distance = length(not_normal_ligth_dir);
	float attenuation = max(point_light.attenuation * distance * distance,1.0f);

	//Calculate base colors
	vec3 diff_color = vec3(split(point_light.diffuse, 0), split(point_light.diffuse, 1), split(point_light.diffuse, 2)) * point_light.diffuse_intensity;
	vec3 spec_color = vec3(split(point_light.specular, 0), split(point_light.specular, 1), split(point_light.specular, 2)) * point_light.specular_intensity;

	
	//final colors for diffuse and specular
	vec3 diffuse_final = max(0, dot(normal, light_dir)) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, normal), 0.000001), point_light.specular_intensity * mat_spec_pow);
	
	//**SHADOWS**//
	float visibility = 1.0f;
	if (shadows_enabled > 0.5f){
		vec3 light_direction = world_position - point_light.position;

		//This sampling with a vec4 automatically compares the sampled value with the forth parameter (i think).
		//So the result is the visibility
		float current_depth = vector_to_depth(light_direction, 1.0, max_fov);
		float bias = 0.0005;
		visibility = texture(shadow_cube, vec4(light_direction, current_depth - bias));
	}

		

    //fragment color
	fracolor =  visibility *  (1.0 - flatness) * (specular_final * mat_spec + diffuse_final * mat_diff) / attenuation;
}

float split(uint color, int n){
	int index = n * 8;
	return bitfieldExtract(color, index, 8) / 256.0f;
}


