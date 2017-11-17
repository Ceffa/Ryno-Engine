E(Includes/global)

uniform sampler2D source_buffer;


void main(){
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);

	gl_FragColor = texture(source_buffer, uv_coords);
	
}