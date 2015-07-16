#version 330

in  vec3 middle_color;
in  mat3 TBN;
in  vec2 middle_uv;

layout(location = 0) out vec3 out_diffuse;
layout(location = 1) out vec2 out_normal;
layout(location = 2) out vec2 out_depth;


uniform sampler2D texture_sampler;
uniform sampler2D normal_map_sampler;


void main() {
	out_diffuse = middle_color *  texture(texture_sampler, middle_uv).xyz;
	out_normal =  (TBN * normalize(texture(normal_map_sampler, middle_uv).rgb*2.0 - 1.0)).xy;
	out_depth = vec2(gl_FragCoord.z,0);
}
