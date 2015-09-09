#version 430

uniform sampler2D m_texture;

in vec4 color;
in vec2 uv;

out vec4 out_color;

void main(){

	out_color = texture(m_texture, uv).a * color.rgba + vec4(0,0,0,0.3);
	
	
}