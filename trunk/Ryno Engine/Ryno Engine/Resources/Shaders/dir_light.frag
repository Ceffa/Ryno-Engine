#version 330

struct DirectionalLight{
	vec4 diffuse;	//Intensity in alpha
	vec4 specular;	//Intensity in alpha
	vec4 ambient;	//Intensity in alpha
	vec3 direction; 
};

//uniform sampler2D g_position_tex;
uniform sampler2D g_color_tex;
uniform sampler2D g_normal_tex;
uniform sampler2D g_depth_tex;


//Inverse projection matrix to get position from depth
uniform mat4 inverse_P_matrix;
uniform DirectionalLight dir_light;

uniform int screen_width;
uniform int screen_height;

out vec3 frag_color;



void main(){
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	float depth = texture(g_depth_tex, uv_coords).r *2.0 - 1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);
	vec4 position_world_space = inverse_P_matrix * position_screen_space;
	vec3 g_position = position_world_space.xyz / position_world_space.w;

	//Get color and flatness from g buffer
	vec4 g_RGBF = texture(g_color_tex, uv_coords);
	vec3 g_color = g_RGBF.rgb;
	float g_flatness = g_RGBF.a;

	//Get normal (and rebuilt it's z axis) from g buffer
	vec2 n = texture(g_normal_tex, uv_coords).xy;
	vec3 g_normal = vec3(n.x, n.y, sqrt(1 - dot(n.xy, n.xy)));

	//Important vectors
	vec3 view_dir = normalize(-g_position);
	vec3 half_dir = normalize(normalize(dir_light.direction) + view_dir);

	//Calculate base colors
	vec3 diff_color = dir_light.diffuse.xyz * dir_light.diffuse.w;
	vec3 spec_color = dir_light.specular.xyz * dir_light.specular.w;

	//final colors for diffuse, specular and ambient
	vec3 diffuse_final = max(0, dot(g_normal, dir_light.direction)) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, g_normal), 0.000001), dir_light.specular.w) ;
	vec3 amb_final = dir_light.ambient.xyz * dir_light.ambient.w;

	//fragment color
	frag_color = g_flatness * g_color + (1.0-g_flatness)*g_color *(specular_final + diffuse_final + amb_final);

}
