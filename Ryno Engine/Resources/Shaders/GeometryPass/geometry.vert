#version 430 
layout(location = 0) in vec3 fv3_Position;
layout(location = 1) in vec2 fv2_Uv;
layout(location = 2) in vec3 fv3_Normal;
layout(location = 3) in vec3 fv3_Tangent;
layout(location = 4) in mat4 fm4_M;
layout(location = 8) in vec2 fv2_Tiling;
layout(location = 9) in vec4 bv4_Color;


out  vec4 middle_color;
out  mat3 TBN;
out  vec2 middle_uv;

uniform mat4 V;
uniform mat4 VP;

void main(){

	mat4 MVP = VP * fm4_M;
	mat4 MV = V * fm4_M;

	gl_Position = MVP * vec4(fv3_Position, 1);
	middle_uv = fv2_Uv * fv2_Tiling;
	middle_color = bv4_Color;

	vec3 normal = normalize((MV * vec4(fv3_Normal, 0)).xyz);
	vec3 tangent = normalize((MV * vec4(fv3_Tangent, 0)).xyz);
	TBN = mat3(tangent, cross(normal, tangent), normal);
}
