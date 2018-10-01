/* Modified from in2gpu.com/2014/07/12/tessellation-tutorial-opengl-4-3 */

#version 400

layout(triangles, fractional_even_spacing, ccw) in;

in TESC_OUT {
    vec3 normal;
    vec2 texture_coords;
} tesc_in[];

out TESE_OUT {
    vec4 position;
    vec3 normal;
    vec2 texture_coords;
    vec4 shadow_position;
    float height;
} tesc_out;

uniform mat4 model;
uniform mat4 model_view;
uniform mat4 model_view_perspective;
uniform mat3 normal_matrix;
uniform mat4 depth_matrix;

vec2 lerp(vec2 v0, vec2 v1, vec2 v2) {
    return (vec2(gl_TessCoord.x)*v0)+(vec2(gl_TessCoord.y)*v1)+(vec2(gl_TessCoord.z) * v2);
}
vec3 lerp(vec3 v0, vec3 v1, vec3 v2) {
    return (vec3(gl_TessCoord.x)*v0)+(vec3(gl_TessCoord.y)*v1)+(vec3(gl_TessCoord.z)*v2);
}

vec4 lerp(vec4 v0, vec4 v1, vec4 v2) {
    return (vec4(gl_TessCoord.x)*v0)+(vec4(gl_TessCoord.y)*v1)+(vec4(gl_TessCoord.z)*v2);
}

void main() {
    tesc_out.position = lerp(
        gl_in[0].gl_Position,
        gl_in[1].gl_Position,
        gl_in[2].gl_Position
    );

    tesc_out.normal = normal_matrix * lerp(
        tesc_in[0].normal,
        tesc_in[1].normal,
        tesc_in[2].normal
    );

    tesc_out.texture_coords = lerp(
        tesc_in[0].texture_coords,
        tesc_in[1].texture_coords,
        tesc_in[2].texture_coords
    );

    tesc_out.height = tesc_out.position.y;
    tesc_out.shadow_position = depth_matrix * model * tesc_out.position;
    gl_Position = model_view_perspective * tesc_out.position;
}
