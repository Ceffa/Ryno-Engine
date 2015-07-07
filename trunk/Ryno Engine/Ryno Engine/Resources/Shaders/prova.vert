#version 330 
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in mat4 in_MV;
layout(location = 8) in mat4 in_MVP;
layout(location = 12) in vec4 in_Color;


out  vec3 middle_pos;
out  vec3 middle_color;
out  vec3 middle_normal;
out  vec3 middle_tangent;
out  vec2 middle_uv;

void main(){
	gl_Position = in_MVP * vec4(in_Position, 1);
	middle_pos = (in_MV * vec4(in_Position, 1)).xyz;
	middle_normal = normalize((in_MV * vec4(in_Normal,0)).xyz);
	middle_uv = in_Uv;
	middle_color = in_Color.rgb;
	middle_tangent = normalize((in_MV * vec4(in_Tangent, 0)).xyz);
}