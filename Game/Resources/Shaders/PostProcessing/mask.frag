#version 430

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D normal_tex;
uniform sampler2D depth_tex;
uniform sampler2D scene_tex;

uniform int mask;


out vec3 fracolor;

layout(std140) uniform glob {
	mat4 V;
	mat4 iV;
	mat4 P;
	mat4 iP;
	mat4 VP;
	mat4 iVP;
	vec4 cameraPos;
	float time;
	int screen_width;
	int screen_height;
};


struct FragmentData{

	vec3 diff_color; 	float flatness;
	vec3 spec_color; 	float spec_pow;
	vec3 normal;	 	float depth;
	vec3 scene_color;								float _pad0;
	vec4 position_view_space;
	vec4 position_world_space;
	vec2 uv_coords;									float _pad1; float _pad2;
};


FragmentData get_fragment_data(){

	FragmentData data;
	
	data.uv_coords = gl_FragCoord.xy / vec2(screen_width,screen_height);
	data.depth = texture(depth_tex, data.uv_coords).r *2.0 - 1.0;
	vec4 position_screen_space = vec4(data.uv_coords * 2.0 - 1.0, data.depth, 1);
	vec4 position_view_space = iP * position_screen_space;
	data.position_view_space = vec4(position_view_space.xyz / position_view_space.w,1);
	data.position_world_space = iV * data.position_view_space;
	
	vec4 sample_diff = texture(diffuse_tex, data.uv_coords);
	data.diff_color = sample_diff.rgb;
	data.flatness = sample_diff.w;
	vec4 sample_spec = texture(specular_tex, data.uv_coords);
	data.spec_color = sample_spec.rgb;
	data.spec_pow = sample_spec.w;
	data.scene_color = texture(scene_tex, data.uv_coords).rgb;

	vec2 n = texture(normal_tex, data.uv_coords).xy;
	data.normal = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));
	
	return data;	
}

void main(){
	
	FragmentData data = get_fragment_data();
	vec3 mask_color = vec3((mask / 4) % 2, (mask / 2) % 2, mask % 2);
	fracolor = data.scene_color * mask_color;
	
}






