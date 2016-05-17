#version 430
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in mat4 in_M;
layout(location = 8) in vec2 in_Tiling;
layout(location = 9) in uint in_DiffuseColor;
layout(location = 10) in uint in_SpecularColor;


out  vec4 middle_diffuse_color;
out  vec4 middle_specular_color;

out  mat3 TBN;
out  vec2 middle_uv;

uniform mat4 g_V;
uniform mat4 g_VP;


float split(uint color, int n);

void main(){


	middle_diffuse_color = vec4(split(in_DiffuseColor, 0), split(in_DiffuseColor, 1), split(in_DiffuseColor, 2), split(in_DiffuseColor, 3));
	middle_specular_color = vec4(split(in_SpecularColor, 0), split(in_SpecularColor, 1), split(in_SpecularColor, 2), split(in_SpecularColor, 3));
	
		
	mat4 MVP = g_VP * in_M;

	gl_Position = MVP * vec4(in_Position, 1);
	middle_uv = in_Uv * in_Tiling;
	vec3 tangent = normalize(vec3(in_M * vec4(in_Tangent,0)));
	vec3 normal = normalize(vec3(in_M * vec4(in_Normal,0)));
	TBN = mat3(tangent, cross(normal, tangent), normal);
}

float split(uint color, int n){
	int index =  n * 8;
	return bitfieldExtract(color, index, 8) / 256.0f;
}