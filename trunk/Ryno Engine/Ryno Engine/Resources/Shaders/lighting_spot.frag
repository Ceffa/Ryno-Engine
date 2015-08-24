#version 330

//Structures

//4-byte aligned
struct SpotLight{
	vec4 position_and_attenuation; //Needs to be divided
	vec4 direction_and_cutoff;
	vec4 diffuse;
	vec4 specular;
};

//Unifroms taken by the buffers
//uniform sampler2D g_position_tex;
uniform sampler2D g_color_tex;
uniform sampler2D g_normal_tex;
uniform sampler2D g_depth_tex;
uniform sampler2DShadow shadow_tex;

//Inverse matrix to rebuild position from depth
uniform mat4 inverse_P_matrix;
uniform mat4 inverse_VP_matrix;
uniform mat4 light_VP_matrix;

uniform mat4 V_matrix;
//All the point light uniforms
uniform SpotLight spot_light;
//Screen size uniforms
uniform int screen_width;
uniform int screen_height;

//Max fov of the light, to reconstruct depth correctly
uniform float max_fov;

out vec3 frag_color;


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
	float depth = texture(g_depth_tex, uv_coords).r *2.0-1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);
	vec4 position_view_space = inverse_P_matrix * position_screen_space;
	vec3 g_position = position_view_space.xyz / position_view_space.w;

	vec4 position_world_space = inverse_VP_matrix * position_screen_space;
	vec4 position_light_MVP_matrix = light_VP_matrix * position_world_space;
	vec3 position_light_MVP_matrix_norm = position_light_MVP_matrix.xyz / position_light_MVP_matrix.w;

	vec4 view_world_pos = V_matrix * vec4(spot_light.position_and_attenuation.xyz, 1);

	//Color directly from g buffer
	vec4 g_RGBF = texture(g_color_tex, uv_coords);
	vec3 g_color = g_RGBF.rgb;
	float g_flatness = g_RGBF.w;
	
	//Normal z-axis built back from the other two
	vec2 n = texture(g_normal_tex, uv_coords).xy;
	vec3 g_normal = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));

	//Important vectors
	vec3 not_normal_ligth_dir = view_world_pos.xyz - g_position;
	vec3 light_dir = normalize(not_normal_ligth_dir);
	vec3 view_dir = normalize(-g_position);
	vec3 half_dir = normalize(light_dir + view_dir);

	//Calculate attenuation
	float distance = length(not_normal_ligth_dir);
	float attenuation = max(spot_light.position_and_attenuation.w * distance * distance,1.0f);

	//Calculate base colors
	vec3 diff_color = spot_light.diffuse.w * spot_light.diffuse.xyz;
	vec3 spec_color = spot_light.specular.w * spot_light.specular.xyz;
	
	//final colors for diffuse and specular
	vec3 diffuse_final = max(0, dot(g_normal, light_dir)) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, g_normal), 0.000001), spot_light.specular.w);
	
	//**SHADOWS**//

	//SHADOWS
	float visibility = texture(shadow_tex, vec3(position_light_MVP_matrix_norm.xy, position_light_MVP_matrix_norm.z));
	
	

		

    //fragment color
	frag_color = visibility *  (1.0 - g_flatness) * g_color * (specular_final + diffuse_final) / attenuation;
}


