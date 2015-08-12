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
uniform sampler2D g_shadow_tex;

//Inverse projection matrix to get position from depth
uniform mat4 inverse_P_matrix;
uniform mat4 inverse_VP_matrix;
uniform mat4 light_VP_matrix;
uniform DirectionalLight dir_light;

uniform int screen_width;
uniform int screen_height;

out vec3 frag_color;



void main(){
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	float depth = texture(g_depth_tex, uv_coords).r *2.0 - 1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);

	vec4 position_view_space = inverse_P_matrix * position_screen_space;
	vec3 g_position = position_view_space.xyz / position_view_space.w;
	
	vec4 position_world_space = inverse_VP_matrix * position_screen_space;
	vec4 position_light_ortho_matrix = light_VP_matrix * position_world_space;
	vec3 position_light_ortho_matrix_norm = position_light_ortho_matrix.xyz / position_light_ortho_matrix.w;

	

	
	
	

	//Get color and flatness from g buffer
	vec4 g_RGBF = texture(g_color_tex, uv_coords);
	vec3 g_color = g_RGBF.rgb;
	float g_flatness = g_RGBF.a;

	//Get normal (and rebuilt it's z axis) from g buffer
	vec2 n = texture(g_normal_tex, uv_coords).xy;
	vec3 g_normal = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));

	//Important vectors
	vec3 view_dir = normalize(-g_position);
	vec3 half_dir = normalize(normalize(dir_light.direction) + view_dir);

	//Calculate base colors
	vec3 diff_color = dir_light.diffuse.xyz * dir_light.diffuse.w;
	vec3 spec_color = dir_light.specular.xyz * dir_light.specular.w;

	//final colors for diffuse, specular and ambient
	float dotNL = max(0, dot(g_normal, dir_light.direction));
	vec3 diffuse_final =  dotNL * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, g_normal), 0.0001), dir_light.specular.w) ;
	vec3 amb_final = dir_light.ambient.xyz * dir_light.ambient.w;

	//shadows
	float visibility = min(1.0, diffuse_final.x+1);
	float bias = 0.005;// *tan(acos(dotNL));

	vec2 poissonDisk[4] = vec2[](
		vec2(-0.94201624, -0.39906216),
		vec2(0.94558609, -0.76890725),
		vec2(-0.094184101, -0.92938870),
		vec2(0.34495938, 0.29387760)
		);

	float shad;
	float dep;
	for (int i = 0; i < 4; i++){
		shad = texture(g_shadow_tex, position_light_ortho_matrix_norm.xy + poissonDisk[i] / 700.0).x;
		dep = position_light_ortho_matrix_norm.z - bias;
		if ( shad < dep){

			visibility -= 0.1;
		}
	}
	
	//fragment color
	frag_color = g_flatness * g_color + (1.0 - g_flatness)*g_color *(amb_final + visibility *( diffuse_final + specular_final));

}
