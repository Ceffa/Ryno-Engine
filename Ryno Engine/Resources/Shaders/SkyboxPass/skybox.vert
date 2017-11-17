E(Includes/global)

in vec3 in_Position;

uniform uint background;
out vec3 coords_3d;
out vec3 color;




void main()
{
	gl_Position = P * vec4((V *  vec4(in_Position, 0.0)).xyz,1.0);
	coords_3d = vec3(in_Position.xy,-in_Position.z); // true because centered in 0
	color = split3(background);
}
