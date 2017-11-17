E(Includes/global)

//4-byte aligned
struct SpotLight{
	vec3 position; //Needs to be divided
	float attenuation;
	vec3 direction;
	float cutoff;
	uint diffuse;
	uint specular;
	float diffuse_intensity;
	float specular_intensity;
	uint blur;
	float shadow_strength;
};

//Unifroms taken by the buffers
//uniform sampler2D position_tex;
uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;
uniform sampler2D depth_tex;
uniform sampler3D jitter;
uniform sampler2DShadow shadow_tex;
uniform int shadows_enabled;

//Inverse matrix to rebuild position from depth
uniform mat4 light_VP_matrix;



//All the spot light uniforms
uniform SpotLight spot_light;


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
	ivec2 coords = ivec2(gl_FragCoord.xy);
	vec2 ndc_coords = (vec2(coords) / vec2(screen_width, screen_height))* 2.0 - 1.0;

	//Rebuild position from depth
	float depth = texelFetch(depth_tex, coords,0).r *2.0-1.0;
	vec4 position_screen_space = vec4(ndc_coords, depth, 1);
	vec4 position_view_space_not_normalized = iP * position_screen_space;
	vec3 position_view_space = position_view_space_not_normalized.xyz / position_view_space_not_normalized.w;
	vec4 position_world_space_not_normalized = iVP * position_screen_space;
	vec3 position_world_space = position_world_space_not_normalized.xyz / position_world_space_not_normalized.w;
	vec4 light_world_space = vec4(spot_light.position, 1);
	vec4 light_view_space = V * light_world_space;

	vec4 position_light_MVP_matrix = light_VP_matrix * vec4(position_world_space,1);
	vec3 position_light_MVP_matrix_norm = position_light_MVP_matrix.xyz / position_light_MVP_matrix.w;
	

	//Color directly from g buffer
	vec4 sample_diff = texelFetch(diffuse_tex, coords,0);
	vec3 mat_diff = sample_diff.rgb;
	float flatness = sample_diff.w;
	vec4 sample_spec = texelFetch(specular_tex, coords,0);
	vec3 mat_spec = sample_spec.rgb;
	float mat_spec_pow = sample_spec.w;
	
	//Normal z-axis built back from the other two
	vec2 n = texelFetch(normal_tex, coords,0).xy;
	vec3 normal_view_space = vec3(n.x, n.y, sqrt(1 - abs(dot(n.xy, n.xy))));

	//Important vectors
	vec4 light_dir_world_space_not_normalized = light_world_space - vec4(position_world_space,1);
	vec4 light_dir_world_space = normalize(light_dir_world_space_not_normalized);
	vec4 light_dir_view_space = itV * light_dir_world_space;
	vec4 view_dir_view_space = vec4(normalize(-position_view_space),0);
	vec4 half_dir_view_space = vec4(normalize(light_dir_view_space.xyz + view_dir_view_space.xyz),0);

	//Calculate attenuation
	float distance = length(light_dir_world_space_not_normalized);
	float attenuation = max(spot_light.attenuation * distance * distance,1.0f);

	//Calculate base colors
	vec3 diff_color = vec3(split(spot_light.diffuse, 0), split(spot_light.diffuse, 1), split(spot_light.diffuse, 2)) * spot_light.diffuse_intensity;
	vec3 spec_color = vec3(split(spot_light.specular, 0), split(spot_light.specular, 1), split(spot_light.specular, 2)) * mat_spec_pow;

	
	//final colors for diffuse and specular
	float dotNL = dot(normal_view_space, light_dir_view_space.xyz);
	vec3 diffuse_final = max(0, dotNL) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir_view_space.xyz, normal_view_space), 0.000001),  spot_light.specular_intensity);
	
	//SHADOWS
	float blur_width = .01;
	float shadow = 1;

	if (shadows_enabled > 0.5){
		
		//If no blur, just calculate shadow
		if (spot_light.blur == 0){
			float bias = 0.0005;
		
			shadow = texture(shadow_tex, vec3(position_light_MVP_matrix_norm.xy, position_light_MVP_matrix_norm.z - bias));
		}else
		{
		
			//Shadows blur info
			uint SIZE = spot_light.blur * 2 + 2;
			uint HALF_SIZE = SIZE/2;
			uint SAMPLES_COUNT = SIZE * SIZE;
			uint SAMPLES_COUNT_DIV_2 = SAMPLES_COUNT/2;
			float INV_SAMPLES_COUNT = (1.0f / SAMPLES_COUNT);
			
			shadow = 0;
			vec4 shadowMapPos = position_light_MVP_matrix;
			vec4 smCoord = shadowMapPos;
			float fsize = smCoord.w * blur_width;
			vec2 jxyscale = vec2(1,1);
			vec3 jcoord = vec3(gl_FragCoord.xy * jxyscale, 0);


			//Sample the outher eight shadows
			for (int i = 0; i<HALF_SIZE; i++) {

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

		shadow = min(1,(1-spot_light.shadow_strength) + shadow);
	}

	
	//CONE CUTOFF (with smoothing to the edges, because I CAN

	float actual_cutoff = dot(normalize(position_world_space - light_world_space.xyz), spot_light.direction);


	
	
	if (actual_cutoff < spot_light.cutoff)
		shadow = 0;
	else if (actual_cutoff < spot_light.cutoff * 1.1)
		shadow *= mix(0, 1, (actual_cutoff - spot_light.cutoff) / (spot_light.cutoff * 0.1));

		

    //fragment color
	fracolor =  flatness * mat_diff + shadow *  (1.0 - flatness) * (mat_spec * specular_final + mat_diff * diffuse_final) / attenuation;
	//fracolor = clean_color(fracolor);
}


