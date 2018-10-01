#version 400

in vec3 texture_coords;

out vec4 colour;

uniform samplerCube tex;

void main() {    
    colour = texture(tex, texture_coords);
}
