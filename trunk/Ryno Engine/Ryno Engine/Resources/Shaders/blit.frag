#version 330

uniform sampler2D source_buffer;
uniform int screen_width;
uniform int screen_height;


void main(){
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);

	float d = texture(source_buffer, uv_coords).x;
	gl_FragDepth = 1-d;
	
}