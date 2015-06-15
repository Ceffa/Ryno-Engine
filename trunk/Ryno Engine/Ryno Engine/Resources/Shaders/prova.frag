#version 330

in  vec2 middle_uv;
in  vec3 middle_color;

out vec4 gl_FragColor;

uniform sampler2D myTextureSampler;

 
void main() {
	gl_FragColor = vec4(middle_color,1.0) + texture( myTextureSampler, middle_uv );

}
