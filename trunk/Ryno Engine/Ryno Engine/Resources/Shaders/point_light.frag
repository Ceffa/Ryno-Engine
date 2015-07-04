#version 330

//Structures

//4-byte aligned baby!
struct PointLight{
	vec4 position_and_attenuation; //Needs to be divided
	vec4 diffuse;
	vec4 specular;
};

//Unifroms taken by the buffers
uniform sampler2D g_position_tex;
uniform sampler2D g_color_tex;
uniform sampler2D g_normal_tex;
//All the point light uniforms
uniform PointLight point_light;
//Screen size uniforms
uniform int screen_width;
uniform int screen_height;


out vec3 frag_color;


void main(){
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);
	
	//Get data from the gbuffer
	vec3 g_position = texture(g_position_tex, uv_coords).xyz;
	vec3 g_color = texture(g_color_tex, uv_coords).xyz;
	vec3 g_normal = texture(g_normal_tex, uv_coords).xyz;
	
	//Important vectors
	vec3 not_normal_ligth_dir = point_light.position_and_attenuation.xyz - g_position;
	vec3 light_dir = normalize(not_normal_ligth_dir);
	vec3 view_dir = normalize(-g_position);
	vec3 half_dir = normalize(light_dir + view_dir);

	//Calculate attenuation
	float distance = length(not_normal_ligth_dir);
	float attenuation = max(point_light.position_and_attenuation.w * distance * distance,1.0f);

	//Calculate base colors
	vec3 diff_color = point_light.diffuse.w * point_light.diffuse.xyz;
	vec3 spec_color = point_light.specular.w * point_light.specular.xyz;
	
	//final colors for diffuse and specular
	vec3 diffuse_final = max(0, dot(g_normal, light_dir)) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, g_normal), 0), point_light.specular.w);
	
    //fragment color
	frag_color = g_color * (specular_final + diffuse_final) / attenuation;
}