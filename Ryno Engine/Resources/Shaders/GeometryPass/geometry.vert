#version 430 
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in mat4 in_M;
layout(location = 8) in vec2 in_Tiling;
layout(location = 9) in vec4 in_Color;


out  vec4 middle_color;
out  mat3 TBN;
out  vec2 middle_uv;

uniform mat4 V;
uniform mat4 VP;

void main(){

	mat4 MVP = VP * in_M;
	mat4 MV = V * in_M;

	gl_Position = MVP * vec4(in_Position, 1);
	middle_uv = in_Uv * in_Tiling;
	middle_color = in_Color;

	vec3 normal = normalize((MV * vec4(in_Normal, 0)).xyz);
	vec3 tangent = normalize((MV * vec4(in_Tangent, 0)).xyz);
	TBN = mat3(tangent, cross(normal, tangent), normal);
}
