#version 330

uniform sampler2D m_pos;
uniform sampler2D m_col;
uniform sampler2D m_nor;

uniform vec3 dir_light;
uniform vec3 dir_color;
uniform float dir_intensity;

out vec3 frag_color;



void main(){
	vec2 TexCoord = gl_FragCoord.xy / vec2(800, 600);
	vec3 WorldPos = texture(m_pos, TexCoord).xyz;
	vec3 Color = texture(m_col, TexCoord).xyz;

	vec3 Normal = texture(m_nor, TexCoord).xyz;
	//Normal = normalize(Normal);
	
	vec4 pos =  vec4(WorldPos, 1);///col
	vec4 col =  vec4(Color, 1);///norm
	vec3 nor =  normalize(Normal);///pos


	frag_color = (max(0, dot(nor, normalize(dir_light))) *  col.xyz * dir_color * dir_intensity) + pos.xyz * 0.00001;
}