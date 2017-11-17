E(Includes/global)

uniform sampler2D source_buffer;



void main(){
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);

	float d = texture(source_buffer, uv_coords).x;
	gl_FragDepth = 1-d;
	
}