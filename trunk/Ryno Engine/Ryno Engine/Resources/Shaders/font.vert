#version 430

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in mat3 in_M;
layout(location = 5) in vec4 in_Color;
layout(location = 6) in float in_Depth;

out vec4 color;
out vec2 uv;
out float depth;

void main()
{
	gl_Position =  vec4(in_M * vec3(in_Position,1.0), 1.0);
	uv = in_Uv;
	color = in_Color;
	depth = in_Depth;

}