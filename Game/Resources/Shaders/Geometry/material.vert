#version 430
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in mat4 in_M;
layout(location = 8) in uint diffuse_color;
layout(location = 9) in uint specular_color;


out  vec4 diff_color;
out  vec4 spec_color;

out  mat3 TBN;
out  vec2 uvs;

uniform mat4 g_V;
uniform mat4 g_VP;


float split(uint color, int n);

void main(){


	vec4 split_diffuse = vec4(split(diffuse_color, 0), split(diffuse_color, 1), split(diffuse_color, 2), split(diffuse_color, 3))/255.0;
	vec4 split_specular = vec4(split(specular_color, 0), split(specular_color, 1), split(specular_color, 2), split(specular_color, 3))/255.0;
	
	mat4 MVP = g_VP * in_M;

	gl_Position = MVP * vec4(in_Position, 1);

	uvs = in_Uv;
	diff_color = split_diffuse;
	spec_color = split_specular;

	vec3 tangent = normalize(mat3(in_M) * in_Tangent);
	vec3 normal = normalize(mat3(in_M)*in_Normal);
	TBN = mat3(tangent, cross(normal, tangent), normal);
}

float split(uint color, int n){
	int index =  n * 8;
	return bitfieldExtract(color, index, 8);
}
