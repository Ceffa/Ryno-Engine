#version 430

layout(location = 0) out float depth;




void main() {

		gl_FragDepth = gl_FragCoord.z;
		depth = gl_FragDepth;

}
