/* Modified from in2gpu.com/2014/06/27/stitching-and-lod-using-tessellation-shaders-for-terrain-rendering */

#version 400

layout(vertices = 3) out;

in VS_OUT {
    vec3 normal;
    vec2 texture_coords;
} tesc_in[];

out TESC_OUT {
    vec3 normal;
    vec2 texture_coords;
} tesc_out[];

uniform vec3 camera_position;

/* Return level of tessellation based on distance */
float tess_level(vec4 pos1, vec4 pos2) {
    float d = distance(pos1, pos2);
    if(d < 2.0) { return 6; }
    if(d < 4.0) { return 5; }
    if(d < 6.0) { return 4; }
    return 1.0;
}
float tess_level(vec3 pos1, vec3 pos2) {
    return tess_level(vec4(pos1, 1.0), vec4(pos2, 1.0));
}

/* Sets tessellation levels */
void set_LoD() {
    vec4 d1 = vec4(gl_in[1].gl_Position.xyz + (gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz) / 2, 1.0);
    vec4 d2 = vec4(gl_in[0].gl_Position.xyz + (gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz) / 2, 1.0);
    vec4 d3 = vec4(gl_in[0].gl_Position.xyz + (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz) / 2, 1.0);

    vec4 camera_position_v4 = vec4(camera_position, 1.0);
    float e0 = tess_level(d1, camera_position_v4);
    float e1 = tess_level(d2, camera_position_v4);
    float e2 = tess_level(d3, camera_position_v4);
    float m = min(e0, min(e1, e2));

    gl_TessLevelInner[0] = floor((min(e0, min(e1, e2)) + max(e0, max(e1, e2))) / 2);
    gl_TessLevelOuter[0] = e0;
    gl_TessLevelOuter[1] = e1;
    gl_TessLevelOuter[2] = e2;
}

void main() { 
    if(gl_InvocationID == 0) {
        set_LoD();
    }

    tesc_out[gl_InvocationID].normal = tesc_in[gl_InvocationID].normal;
    tesc_out[gl_InvocationID].texture_coords = tesc_in[gl_InvocationID].texture_coords;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
