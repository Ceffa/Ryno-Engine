E(Includes/global)
E(Includes/dir_include)

uniform sampler3D jitter;

uniform sampler2DShadow shadow_tex;

//Inverse projection matrix to get position from depth
layout(std140, binding = 1) uniform dir_ubo {
	DirectionalLight dir_light;
};

out vec3 fracolor;


void main(){
	
	//fragment color
	fracolor = get_shaded_fragment(dir_light, gl_FragCoord.xy);

}


float get_shadow(vec4 position_world_space, float dotNL) {

	//SHADOWS
	float blur_width = .0012;
	float shadow = 1;

		vec4 position_light_ortho_matrix = dir_light.light_VP_matrix * position_world_space;
		vec3 position_light_ortho_matrix_norm = position_light_ortho_matrix.xyz / position_light_ortho_matrix.w;


		if (dir_light.blur == 0) {
			float bias = 0.0005;

			shadow = texture(shadow_tex, vec3(position_light_ortho_matrix_norm.xy, position_light_ortho_matrix_norm.z - bias));
		}
		else
		{

			//Shadows blur info
			uint SIZE = dir_light.blur * 2 + 2;
			uint HALF_SIZE = SIZE / 2;
			uint SAMPLES_COUNT = SIZE * SIZE;
			uint SAMPLES_COUNT_DIV_2 = SAMPLES_COUNT / 2;
			float INV_SAMPLES_COUNT = (1.0f / SAMPLES_COUNT);



			shadow = 0;
			vec4 shadowMapPos = position_light_ortho_matrix;
			vec4 smCoord = shadowMapPos;
			float fsize = smCoord.w * blur_width;
			vec2 jxyscale = vec2(1, 1);
			vec3 jcoord = vec3(gl_FragCoord.xy * jxyscale, 0);


			//Sample the outher eight shadows
			for (uint i = 0; i < HALF_SIZE; i++) {

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

				for (int i = 0; i < SAMPLES_COUNT_DIV_2 - 4; i++) {

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

		return min(1, (1 - dir_light.shadow_strength) + shadow);
}

