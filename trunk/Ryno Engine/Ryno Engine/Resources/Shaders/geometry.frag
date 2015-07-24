#version 430
//#extension GL_EXT_gpu_shader4 : enable

in  vec4 middle_color;
in  mat3 TBN;
in  vec2 middle_uv;

layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec2 out_normal;
layout(location = 2) out float out_depth;


uniform sampler2D texture_sampler;
uniform sampler2D normal_map_sampler;


void main() {
	out_diffuse = vec4(middle_color.rgb *  texture(texture_sampler, middle_uv).rgb, middle_color.w);
	//if (textureSize2D(normal_map_sampler,0).x > 0)
	out_normal = (TBN * normalize(texture(normal_map_sampler, middle_uv).rgb*2.0 - 1.0)).xy;
	//else
		//out_normal = vec2(TBN[0][2],TBN[1][2]); //get back the "normal" normal
	
	out_depth = gl_FragCoord.z;
}
