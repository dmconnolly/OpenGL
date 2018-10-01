#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;

out vec2 vs_out_texture_coords;

void main() {
    vs_out_texture_coords = texture_coords;
    gl_Position = vec4(position, 1.0); 
}
