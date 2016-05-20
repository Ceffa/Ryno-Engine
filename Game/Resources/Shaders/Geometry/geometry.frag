#version 430


in  vec4 middle_diffuse_color;
in  vec4 middle_specular_color;
in  mat3 TBN;
in  vec2 middle_uv;

layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec4 out_specular;
layout(location = 2) out vec2 out_normal;
layout(location = 3) out float out_depth;


uniform sampler2D texture_sampler;
uniform sampler2D normal_map_sampler;


void main() {
	out_diffuse = vec4(middle_diffuse_color.rgb *  texture(texture_sampler, middle_uv).rgb, middle_diffuse_color.w);
	out_specular = middle_specular_color;

	
	out_normal = (TBN * normalize(texture(normal_map_sampler, middle_uv).rgb*2.0 - 1.0)).xy;
	
	out_depth = gl_FragCoord.z;
}
