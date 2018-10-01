#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coords;

out VS_OUT {
    vec3 normal;
    vec2 texture_coords;
} vert_out;

void main() {
	vert_out.normal = normal;
	vert_out.texture_coords = texture_coords;
	gl_Position = vec4(position, 1.0);
}
