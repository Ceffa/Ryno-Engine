#version 330

struct DirectionalLight{
	vec4 diffuse;	//Intensity in alpha
	vec4 specular;	//Intensity in alpha
	vec4 ambient;	//Intensity in alpha
	vec3 direction; 
};

uniform sampler2D g_position_tex;
uniform sampler2D g_color_tex;
uniform sampler2D g_normal_tex;

uniform DirectionalLight dir_light;

uniform int screen_width;
uniform int screen_height;

out vec3 frag_color;



void main(){
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	
	//Get data from the gbuffer
	vec3 g_position = texture(g_position_tex, uv_coords).xyz;
	vec3 g_color = texture(g_color_tex, uv_coords).xyz;
	vec3 g_normal = texture(g_normal_tex, uv_coords).xyz;
	
	//Important vectors
	vec3 view_dir = normalize(-g_position);
	vec3 half_dir = normalize(normalize(dir_light.direction) + view_dir);

	//Calculate base colors
	vec3 diff_color = dir_light.diffuse.xyz * dir_light.diffuse.w;
	vec3 spec_color = dir_light.specular.xyz * dir_light.specular.w;

	//final colors for diffuse, specular and ambient
	vec3 diffuse_final = max(0, dot(g_normal, dir_light.direction)) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, g_normal), 0), dir_light.specular.w) ;
	vec3 amb_final = dir_light.ambient.xyz * dir_light.ambient.w;

	//fragment color
	frag_color = g_color * (specular_final + diffuse_final + amb_final);

}