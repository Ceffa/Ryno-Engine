E(Includes/geom_include)

in  vec4 diff_color;
in	vec4 spec_color; 
in  mat3 TBN;
in  vec2 uvs;
in float _alpha;

layout(location = 0) out vec4 out_diffuse;
layout(location = 1) out vec4 out_specular;
layout(location = 2) out vec2 out_normal;
layout(location = 3) out float out_depth;


uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;
uniform sampler2D alpha_texture;



void main() {

	//Discard alpha
	float tex_alpha = textureSize2D(alpha_texture, 0).x > 1 ? texture(alpha_texture, uvs).r : 1;
	if (is_transparent(ivec2(gl_FragCoord.xy), tex_alpha * _alpha))
		discard;
	//Sample textures
	vec3 text_diffuse = textureSize2D(diffuse_texture,0).x > 1 ? texture(diffuse_texture, uvs).rgb	: vec3(0,0,0);
	vec3 text_specular = textureSize2D(specular_texture,0).x > 1 ? texture(specular_texture, uvs).rgb : vec3(1,1,1);
	vec3 text_normal = textureSize2D(normal_texture,0).x > 1 ? texture(normal_texture, uvs).rgb : vec3(.5,.5,1);

	//Calculate final colors
	out_diffuse = vec4(diff_color.rgb * text_diffuse.rgb, diff_color.w);
	out_specular = vec4(spec_color.rgb * text_specular, spec_color.w);

	//Normalize normal
	out_normal = (TBN * normalize(text_normal*2.0 - 1.0)).xy*.5 + 0.5;

	//Set depth
	out_depth = 1-gl_FragCoord.z;
}
