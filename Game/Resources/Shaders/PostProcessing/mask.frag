E(Includes/post_proc)

uniform int mask;

void main(){
	
	FragmentData data = get_fragment_data();
	vec3 mask_color = vec3((mask / 4) % 2, (mask / 2) % 2, mask % 2);
	fracolor = data.scene_color * mask_color;
	
}






