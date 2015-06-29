#version 330

uniform sampler2D m_pos;
uniform sampler2D m_col;
uniform sampler2D m_nor;

uniform vec3 point_light_pos;

out vec3 frag_color;


void main(){
	vec2 TexCoord = gl_FragCoord.xy / vec2(800, 600);
	
	vec3 Color = texture(m_col, TexCoord).xyz;

	vec3 pos = texture(m_pos, TexCoord).xyz;
	vec3 nor = normalize(texture(m_nor, TexCoord).xyz);
	vec4 col = vec4(Color, 1);
	


//	frag_color = max(0, dot(nor,- normalize(pos-point_light_pos))) * col.xyz;
	frag_color = max(0, dot(nor, -normalize(pos-point_light_pos))) * col.xyz + vec3(.2,0,0) + pos.xyz * 0.00001;
}