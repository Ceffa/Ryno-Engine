E(Includes/post_proc)

uniform int mask;

void main(){
	
	vec3 mask_color = vec3((mask / 4) % 2, (mask / 2) % 2, mask % 2);
	fracolor = sample_scene_color(gl_FragCoord.xy) * mask_color;
	
}






