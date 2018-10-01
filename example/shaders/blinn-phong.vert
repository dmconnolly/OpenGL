#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coords;

out VS_OUT {
    vec4 position;
    vec3 normal;
    vec2 texture_coords;
    vec4 shadow_position;
} vs_out;

uniform mat4 model;
uniform mat4 model_view;
uniform mat4 model_view_perspective;
uniform mat3 normal_matrix;
uniform mat4 depth_matrix;

void main() {
	vec4 position_v4 = vec4(position, 1.0);

	vs_out.position = model_view * position_v4;
	vs_out.normal = normal_matrix * normal;
	vs_out.texture_coords = texture_coords;
	vs_out.shadow_position = depth_matrix * model * position_v4;

	gl_Position = model_view_perspective * position_v4;
}
