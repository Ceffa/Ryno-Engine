#version 330

in  vec3 middle_pos;
in  vec3 middle_color;
in  vec3 middle_normal;
in  vec3 middle_tangent;
in  vec2 middle_uv;

layout(location = 0) out vec3 out_pos;
layout(location = 1) out vec3 out_diffuse;
layout(location = 2) out vec3 out_normal;

uniform sampler2D texture_sampler;
uniform sampler2D normal_map_sampler;


vec3 calc_bumped_normal()
{
	//normalize tangent and normal
	vec3 normal = normalize(middle_normal);
	vec3 tangent = normalize(middle_tangent);
	//Gramm-Schmidt, to adapt tangent to the single fragment, 
	//because the normal is interpolated. Or something like that.
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	//bitangent is the cross of unit-length normal and tangent
	vec3 bitangent = cross(tangent, normal);
	//sample normal map
	vec3 bumped_normal = texture(normal_map_sampler, middle_uv).xyz;
	//adapt value to negative numbers
	bumped_normal = 2.0 * bumped_normal - vec3(1.0, 1.0, 1.0);
	//Construct tangent space matrix
	mat3 TBN = mat3(tangent, bitangent, normal);
	vec3 final_normal = normalize(TBN * bumped_normal);
	return final_normal;
}
void main() {
	out_pos = middle_pos;
	out_diffuse = middle_color *  texture(texture_sampler, middle_uv).xyz;
	out_normal = calc_bumped_normal();
}
