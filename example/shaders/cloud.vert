#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;

out VS_OUT {
    vec4 position;
    vec2 texture_coords;
} vs_out;

uniform mat4 model;
uniform mat4 model_view;
uniform mat4 model_view_perspective;

void main() {
	vec4 position_v4 = vec4(position, 1.0);

	vs_out.position = model_view * position_v4;
	vs_out.texture_coords = texture_coords;

	gl_Position = model_view_perspective * position_v4;
}
