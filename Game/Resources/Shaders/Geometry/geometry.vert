E(Includes/global)

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in mat4 in_M;
layout(location = 8) in vec2 in_Tiling;
layout(location = 9) in uint in_DiffuseColor;
layout(location = 10) in uint in_SpecularColor;


out  vec4 middle_diffuse_color;
out  vec4 middle_specular_color;

out  mat3 TBN;
out  vec2 middle_uv;



void main(){


	middle_diffuse_color = split4(in_DiffuseColor);
	middle_specular_color = split4(in_SpecularColor);
	
		
	mat4 MVP = VP * in_M;

	gl_Position = MVP * vec4(in_Position, 1);
	middle_uv = in_Uv * in_Tiling;
	
	mat4 MV = V * in_M;
	vec3 tangent = vec3(normalize(MV *vec4(in_Tangent,0)));
	vec3 normal = vec3(normalize(MV*vec4(in_Normal,0)));
	TBN = mat3(tangent, cross(normal, tangent), normal);
}
