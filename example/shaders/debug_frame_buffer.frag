#version 400

in vec2 vs_out_texture_coords;

out vec4 colour;

uniform sampler2D shadow_texture;

void main() { 
    colour = texture(shadow_texture, vs_out_texture_coords);
}
