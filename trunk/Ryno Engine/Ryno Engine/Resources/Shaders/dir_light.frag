#version 330

struct DirectionalLight{
	vec4 diffuse;	//Intensity in alpha
	vec4 specular;	//Intensity in alpha
	vec4 ambient;	//Intensity in alpha
	vec3 direction; 
};

uniform sampler2D g_color_tex;
uniform sampler2D g_normal_tex;

uniform DirectionalLight dir_light;

uniform int screen_width;
uniform int screen_height;

out vec3 frag_color;



void main(){
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	
	vec3 g_color = texture(g_color_tex, uv_coords).xyz;
	vec3 g_normal = texture(g_normal_tex, uv_coords).xyz;
	
	vec3 diff_color = dir_light.diffuse.xyz * dir_light.diffuse.w;
	vec3 spec_color = dir_light.specular.xyz * dir_light.specular.w;
	vec3 amb_color = dir_light.ambient.xyz * dir_light.ambient.w;

	vec3 final_color = g_color * (diff_color + spec_color);

	frag_color = max(0, dot(g_normal, normalize(dir_light.direction))) *final_color + g_color * amb_color;

}