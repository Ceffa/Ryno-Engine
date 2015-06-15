#version 330 core
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec4 in_Color;

out vec2 middle_uv;
out vec3 middle_color;

void main(){
	gl_Position = vec4(in_Position,1);
	middle_uv = in_Uv;
    middle_color = in_Color.rgb;
}