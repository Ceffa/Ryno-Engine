E(Includes/global)

uniform sampler2D normal_tex;
uniform sampler2D depth_tex;

out float fracolor;

float sample_depth(vec2 c) {
	return (1 - texelFetch(depth_tex, ivec2(c), 0).r) *2.0 - 1.0;
}

vec3 sample_normal(vec2 c) {
	vec2 n = texelFetch(normal_tex, ivec2(c), 0).xy * 2 - 1;
	return vec3(n.x, n.y, sqrt(1 - dot(n, n)));
}
vec3 sample_view_pos(vec2 c) {
	vec2 NDC_xy = (c / vec2(screen_width, screen_height)) * 2 - 1;
	return w_div(iP * vec4(NDC_xy, sample_depth(c), 1));
}


uniform sampler2D noise;

out float visibility;

vec3 get_random_normal();

const int samples = 16;
const vec3 sample_sphere[samples] = {
	vec3(0.5381, 0.1856,-0.4319), vec3(0.1379, 0.2486, 0.4430),
	vec3(0.3371, 0.5679,-0.0057), vec3(-0.6999,-0.0451,-0.0019),
	vec3(0.0689,-0.1598,-0.8547), vec3(0.0560, 0.0069,-0.1843),
	vec3(-0.0146, 0.1402, 0.0762), vec3(0.0100,-0.1924,-0.0344),
	vec3(-0.3577,-0.5301,-0.4358), vec3(-0.3169, 0.1063, 0.0158),
	vec3(0.0103,-0.5869, 0.0046), vec3(-0.0897,-0.4940, 0.3287),
	vec3(0.7119,-0.0154,-0.0918), vec3(-0.0533, 0.0596,-0.5411),
	vec3(0.0352,-0.0631, 0.5460), vec3(-0.4776, 0.2847,-0.0271)
};

void main() {

	vec2 c = gl_FragCoord.xy;
	vec3 view_pos = sample_view_pos(c);
	vec3 norm = sample_normal(c);
	float depth = sample_depth(c);

	float scale = 1;
	float intensity = 12;
	float radius = 25;
	float bias = 0.1;
	float occlusion = 0;

	for (int i = 0; i < samples; i++) {
		vec3 rand_norm = get_random_normal();
		vec3 rand_vec = radius *  reflect(sample_sphere[i], rand_norm);
		vec3 end_of_ray = view_pos + sign(dot(norm, rand_vec)) * rand_vec;
		vec3 NDC_point = w_div(P * vec4(end_of_ray, 1));
		vec2 new_coords = (NDC_point.xy + 1) * vec2(screen_width, screen_height) / 2;
		vec3 s_view_pos = sample_view_pos(new_coords);
		vec3 dv = s_view_pos - view_pos;
		float d = length(dv);
		dv /= d;

		occlusion += (max(0, dot(dv, norm) - bias)* (1 / (1 + d * scale)))*intensity;
	}
	occlusion /= samples;

	visibility = 1 - occlusion;
}

vec3 get_random_normal() {
	ivec2 noise_c = ivec2(mod(gl_FragCoord.xy, 256));
	return normalize(texelFetch(noise, noise_c, 0).rgb * 2 - 1);
}
