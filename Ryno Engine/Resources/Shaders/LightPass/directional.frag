#version 430

struct DirectionalLight{
	uint diffuse;	
	uint specular;	
	uint ambient;	
	float diffuse_intensity;
	float specular_intensity;
	float ambient_intensity;
	vec3 direction;
};

//uniform sampler2D position_tex;
uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;
uniform sampler2D depth_tex;
uniform sampler2DShadow shadow_tex;
uniform int shadows_enabled;

//Inverse projection matrix to get position from depth
uniform mat4 inverse_P_matrix;
uniform mat4 inverse_VP_matrix;
uniform mat4 light_VP_matrix;
uniform DirectionalLight dir_light;

uniform int screen_width;
uniform int screen_height;

out vec3 fracolor;


float split(uint color, int n);

void main(){

	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	float depth = texture(depth_tex, uv_coords).r *2.0 - 1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);

	vec4 position_view_space = inverse_P_matrix * position_screen_space;
	vec3 position = position_view_space.xyz / position_view_space.w;
	
	vec4 position_world_space = inverse_VP_matrix * position_screen_space;
	vec4 position_light_ortho_matrix = light_VP_matrix * position_world_space;
	vec3 position_light_ortho_matrix_norm = position_light_ortho_matrix.xyz / position_light_ortho_matrix.w;
	

	//Get color and flatness from g buffer
	vec4 sample_diff = texture(diffuse_tex, uv_coords);
	vec3 mat_diff = sample_diff.rgb;
	float flatness = sample_diff.w;
	vec4 sample_spec = texture(specular_tex, uv_coords);
	vec3 mat_spec = sample_spec.rgb;
	float mat_spec_pow = sample_spec.w;

	//Get normal (and rebuilt it's z axis) from g buffer
	vec2 n = texture(normal_tex, uv_coords).xy;
	vec3 normal = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));

	//Important vectors
	vec3 view_dir = normalize(-position);
	vec3 half_dir = normalize(normalize(dir_light.direction) + view_dir);

	//Calculate base colors
	vec3 diff_color = vec3(split(dir_light.diffuse, 0), split(dir_light.diffuse, 1), split(dir_light.diffuse, 2)) * dir_light.diffuse_intensity;
	vec3 spec_color = vec3(split(dir_light.specular, 0), split(dir_light.specular, 1), split(dir_light.specular, 2)) * dir_light.specular_intensity;
	vec3 amb_final = vec3(split(dir_light.ambient, 0), split(dir_light.ambient, 1), split(dir_light.ambient, 2)) * dir_light.ambient_intensity;

	//final colors for diffuse, specular and ambient
	float dotNL = max(0, dot(normal, dir_light.direction));
	vec3 diffuse_final =  dotNL * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, normal), 0.0001), dir_light.specular_intensity * mat_spec_pow) ;



	
	
	//SHADOWS

	float visibility = 1.0f;
	vec2 poissonDisk[4];
	if (shadows_enabled > 0.5){
		poissonDisk = vec2[](
			vec2(-0.94201624, -0.39906216),
			vec2(0.94558609, -0.76890725),
			vec2(-0.094184101, -0.92938870),
			vec2(0.34495938, 0.29387760)
			);

		float shadow_strenght = 0.2;
		float bias = 0.0005;




		for (int i = 0; i < 4; i++){
			visibility -= (shadow_strenght * (1.0 - texture(shadow_tex, vec3(position_light_ortho_matrix_norm.xy + poissonDisk[i] / 700.0, position_light_ortho_matrix_norm.z - bias))));
		}
	}
	
	
	
	//fragment color
	fracolor =  flatness * mat_diff + (1.0 - flatness)*(amb_final + visibility *(mat_diff * diffuse_final + mat_spec * specular_final));

}


float split(uint color, int n){
	int index = n * 8;
	return bitfieldExtract(color, index, 8) / 256.0f;
}
