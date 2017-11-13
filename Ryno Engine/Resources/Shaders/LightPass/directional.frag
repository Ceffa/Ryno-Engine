#version 430

struct DirectionalLight{
	mat4 light_VP_matrix;
	mat4 light_V_matrix;
	uint diffuse;	
	uint specular;	
	uint ambient;	
	uint blur;
	float diffuse_intensity;
	float specular_intensity;
	float ambient_intensity;
	float shadow_strength;
	vec3 direction;	float _pad;
};

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

uniform sampler3D jitter;

uniform sampler2DShadow shadow_tex;
uniform int shadows_enabled;

//Inverse projection matrix to get position from depth
uniform DirectionalLight dir_light;


out vec3 fracolor;

layout(std140) uniform glob {
	mat4 V;
	mat4 iV;
	mat4 P;
	mat4 iP;
	mat4 VP;
	mat4 iVP;
	vec4 cameraPos;
	float time;
	int screen_width;
	int screen_height;
};


float split(uint color, int n);

void main(){

	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	float depth = texture(depth_tex, uv_coords).r *2.0 - 1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);

	vec4 position_view_space = iP * position_screen_space;
	vec3 position = position_view_space.xyz / position_view_space.w;
	
	vec4 position_world_space = iVP * position_screen_space;
	vec4 position_light_ortho_matrix = dir_light.light_VP_matrix * position_world_space;
	vec3 position_light_ortho_matrix_norm = position_light_ortho_matrix.xyz / position_light_ortho_matrix.w;
	vec3 view_dir_light = normalize(vec3(dir_light.light_V_matrix * vec4(dir_light.direction,0)));

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
	vec3 half_dir = normalize(view_dir_light + view_dir);

	//Calculate base colors
	vec3 diff_color = vec3(split(dir_light.diffuse, 0), split(dir_light.diffuse, 1), split(dir_light.diffuse, 2)) * dir_light.diffuse_intensity;
	vec3 spec_color = vec3(split(dir_light.specular, 0), split(dir_light.specular, 1), split(dir_light.specular, 2)) * mat_spec_pow;
	vec3 amb_final = vec3(split(dir_light.ambient, 0), split(dir_light.ambient, 1), split(dir_light.ambient, 2)) * dir_light.ambient_intensity;

	//final colors for diffuse, specular and ambient
	float dotNL = max(0, dot(normal, view_dir_light));
	vec3 diffuse_final =  dotNL * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir, normal), 0.0001), dir_light.specular_intensity) ;



	
	
	//SHADOWS
	float blur_width = .0012;
	float shadow = 1;

	if (shadows_enabled > 0.5){

		if (dir_light.blur == 0){
			float bias = 0.0005;
		
			shadow = texture(shadow_tex, vec3(position_light_ortho_matrix_norm.xy, position_light_ortho_matrix_norm.z - bias));
		}
		else
		{
		
			//Shadows blur info
			uint SIZE = dir_light.blur * 2 + 2;
			uint HALF_SIZE = SIZE/2;
			uint SAMPLES_COUNT = SIZE * SIZE;
			uint SAMPLES_COUNT_DIV_2 = SAMPLES_COUNT/2;
			float INV_SAMPLES_COUNT = (1.0f / SAMPLES_COUNT);
			
			shadow = 0;
			vec4 shadowMapPos = position_light_ortho_matrix;
			vec4 smCoord = shadowMapPos;
			float fsize = smCoord.w * blur_width;
			vec2 jxyscale = vec2(1,1);
			vec3 jcoord = vec3(gl_FragCoord.xy * jxyscale, 0);


			//Sample the outher eight shadows
			for (uint i = 0; i<HALF_SIZE; i++) {

				vec4 offset = texture(jitter, jcoord);

				jcoord.z += 1.0f / SAMPLES_COUNT_DIV_2;

				smCoord.xy = offset.xy * fsize + shadowMapPos.xy;

				shadow += textureProj(shadow_tex, smCoord) / SIZE;
			
				smCoord.xy = offset.zw * fsize + shadowMapPos.xy;

				shadow += textureProj(shadow_tex, smCoord) / SIZE;

			  }

			  //If there is a medium value of shadow, continue with the remaining ones
			  if ((shadow - 1) * shadow * dotNL != 0) {

				  // most likely, we are in the penumbra
				  shadow *= 1.0f / SIZE; // adjust running total


				  // refine our shadow estimate

				  for (int i = 0; i<SAMPLES_COUNT_DIV_2 - 4; i++) {

						vec4 offset = texture(jitter, jcoord);

						jcoord.z += 1.0f / SAMPLES_COUNT_DIV_2;


						//The vec 4 holds two informations, doit for both
						smCoord.xy = offset.xy * fsize + shadowMapPos.xy;
						shadow += textureProj(shadow_tex, smCoord)* INV_SAMPLES_COUNT;

						smCoord.xy = offset.zw * fsize + shadowMapPos.xy;
						shadow += textureProj(shadow_tex, smCoord)* INV_SAMPLES_COUNT;

				 }

			}
		}

		shadow = min(1,(1-dir_light.shadow_strength) + shadow);
	}
	
	
	
	//fragment color
	fracolor = amb_final * mat_diff  + flatness * mat_diff + (1.0 - flatness)*shadow *(mat_diff * diffuse_final + mat_spec * specular_final);

}


float split(uint color, int n){
	int index = n * 8;
	return bitfieldExtract(color, index, 8) / 255.0f;
}
