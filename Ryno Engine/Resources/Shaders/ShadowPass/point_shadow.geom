# version 430

uniform mat4 projection_matrices[6];
 
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 18) out;
 
in vec3 pos[];
out vec3 frag_pos;
 
void main() {
     
    for(int j = 0; j < 6; j++) {
        gl_Layer = j;
        for(int i = 0; i < 3; i++) {
            frag_pos = pos[i];
            gl_Position = projection_matrices[j] * vec4(pos[i], 1);
            EmitVertex();
        }
        EndPrimitive();
    }
}


