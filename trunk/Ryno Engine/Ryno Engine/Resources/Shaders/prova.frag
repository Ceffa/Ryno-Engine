#version 330

in  vec2 middle_uv;
in  vec3 middle_normal;
in  vec3 middle_pos;
in vec3 middle_color;

layout(location = 0) out vec3 out_pos;
layout(location = 1) out vec3 out_diffuse;
layout(location = 2) out vec3 out_normal;
layout(location = 3) out vec3 out_tex;

uniform sampler2D myTextureSampler;

 
void main() {
	out_pos = middle_pos;
	out_diffuse = middle_color * texture(myTextureSampler, middle_uv).xyz;
	out_normal = normalize(middle_normal);
	out_tex = vec3(middle_uv, 0.0);
}
