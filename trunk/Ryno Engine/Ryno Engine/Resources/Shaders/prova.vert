#version 150

in  vec2 in_Position;
in  vec3 in_Color;
out vec3 ex_Color;

void main(void) {
	gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
    ex_Color = in_Color;
}
