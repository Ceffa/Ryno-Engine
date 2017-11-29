E(Includes/post_proc)

uniform int mask;

void main() {

	vec2 off[4] = { vec2(0,1), vec2(1,0), vec2(0,-1), vec2(-1,0) };

	vec2 c = gl_FragCoord.xy;
	vec3 view_pos = sample_view_pos(c);
	vec3 norm = sample_normal(c);

	float scale = 50;
	float intensity = 50;
	float radius = 1000;
	float bias = 0.3;
	float occlusion = 0;

	for (int i = 0; i < 4; i++) {
		vec2 s_c = c + off[i]*(radius/view_pos.z);
		vec3 s_view_pos = sample_view_pos(s_c);
		vec3 dv = s_view_pos - view_pos;
		float d = length(dv);
		dv /= d;

		occlusion += (max(0,dot(dv,norm)-bias)* (1 / (1 + d * scale)))*intensity;
	}
	occlusion /= 4;
	
	fracolor = (1 - occlusion) * sample_scene_color(c);


}


