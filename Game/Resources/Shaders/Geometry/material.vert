E(Includes/global)

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Uv;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in mat4 in_M;
layout(location = 8) in uint diffuse_color;
layout(location = 9) in uint specular_color;
layout(location = 10) in float alpha;


out  vec4 diff_color;
out  vec4 spec_color;

out  mat3 TBN;
out  vec2 uvs;
out float _alpha;

uniform int power;


void main(){


	vec4 split_diffuse = split4(diffuse_color);
	vec4 split_specular = split4(specular_color);
	

	mat4 MVP = VP * in_M;

	gl_Position = MVP * vec4(in_Position + vec3(0,power * sin(time / 200)  * sin(in_Position.x /100),0), 1);

	uvs = in_Uv;
	diff_color = split_diffuse;
	spec_color = split_specular;

	mat4 MV = V * in_M;
	vec3 tangent = vec3(normalize(MV * vec4(in_Tangent,0)));
	vec3 normal = vec3(normalize(MV*vec4(in_Normal,0)));
	TBN = mat3(tangent, cross(normal, tangent), normal);

	_alpha = alpha;
}
