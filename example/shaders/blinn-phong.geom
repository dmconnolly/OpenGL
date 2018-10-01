#version 400

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texture_coords;
} gs_in[];

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texture_coords;
} gs_out;

void main() {
    for(int i=0; i<gs_in.length(); ++i) {
        gs_out.position = gs_in[i].position;
        gs_out.normal = gs_in[i].normal;
        gs_out.texture_coords = gs_in[i].texture_coords;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
