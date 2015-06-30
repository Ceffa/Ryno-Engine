#version 330

uniform sampler2D m_pos;
uniform sampler2D m_col;
uniform sampler2D m_nor;

uniform vec3 position;
uniform vec3 color;
uniform float intensity;
uniform float attenuation;

out vec3 frag_color;


void main(){
	vec2 TexCoord = gl_FragCoord.xy / vec2(800, 600);
	
	vec3 Color = texture(m_col, TexCoord).xyz;

	vec3 pos = texture(m_pos, TexCoord).xyz;
	vec3 nor = normalize(texture(m_nor, TexCoord).xyz);
	vec4 col = vec4(Color, 1);
	
	vec3 dir_between = pos - position;
	float dist = length(dir_between);
	float atten = max(attenuation * dist * dist,1.0f);
	vec3 final_col =  intensity * col.xyz * (color.xyz / atten);
	final_col = clamp(final_col, 0.0, 1.0);
	frag_color = max(0, dot(nor, -normalize(dir_between))) * final_col;
}