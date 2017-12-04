E(Includes/global)

uniform sampler2D input_tex;

out float fracolor;

#define SAMPLES 5

float gauss[SAMPLES] = {

	0.153388,	0.221461,	0.250301,	0.221461,	0.153388
};


void main() {

	float f = 0;
	ivec2 low = ivec2(0,-SAMPLES / 2);
	for (int i = 0; i < SAMPLES; i++) {
		f += texelFetch(input_tex, ivec2(gl_FragCoord.xy) + low + ivec2(0,i), 0).r * gauss[i];
	}
	fracolor = f;
}
