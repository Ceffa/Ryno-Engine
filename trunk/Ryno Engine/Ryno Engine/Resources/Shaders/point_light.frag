#version 330

//Structures

//4-byte aligned baby!
struct PointLight{
	vec4 position_and_attenuation; //Needs to be divided
	vec4 diffuse;
	vec4 specular;
};

//Unifroms taken by the buffers
//uniform sampler2D g_position_tex;
uniform sampler2D g_color_tex;
uniform sampler2D g_normal_tex;
uniform sampler2D g_depth_tex;
uniform samplerCube shadow_cube;

//Inverse matrix to rebuild position from depth
uniform mat4 inverse_P_matrix;
//All the point light uniforms
uniform PointLight point_light;
//Screen size uniforms
uniform int screen_width;
uniform int screen_height;


out vec3 frag_color;


void main(){
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);
	
	//Rebuild position from depth
	float depth = texture(g_depth_tex, uv_coords).r *2.0-1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);
	vec4 position_world_space = inverse_P_matrix * position_screen_space;
	vec3 g_position = position_world_space.xyz / position_world_space.w;

	//Color directly from g buffer
	vec4 g_RGBF = texture(g_color_tex, uv_coords);
	vec3 g_color = g_RGBF.rgb;
	float g_flatness = g_RGBF.w;
	
	//Normal z-axis built back from the other two
	vec2 n = texture(g_normal_tex, uv_coords).xy;
	vec3 g_normal = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));

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
	vec3 specular_final = spec_color * pow(max(dot(half_dir, g_normal), 0.000001), point_light.specular.w);
	
	//Shadows
	float visibility = texture(shadow_cube, vec3(0,0,0)).x;


    //fragment color
	frag_color = visibility *  (1.0 - g_flatness) * g_color * (specular_final + diffuse_final) / attenuation;
}
