#version 430

uniform sampler2D m_texture;

in vec4 color;
in vec2 uv;
in float depth;

out vec4 out_color;

void main(){

	out_color = vec4(color.rgb, texture(m_texture, uv).a);
	if (out_color.a < 0.5)
		discard;
	gl_FragDepth = depth;
	
}