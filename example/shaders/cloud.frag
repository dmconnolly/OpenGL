#version 400

in VS_OUT {
    vec4 position;
    vec2 texture_coords;
} fs_in;

out vec4 colour;

uniform sampler2D noise_map;

#define SCALE(x,a1,a2,b1,b2) ((((b2-b1)*(x-a1))/(a2-a1))+b1)

#define MIN_CENTRE_DIST (0.0)
#define MAX_CENTRE_DIST (0.5)
#define MIN_ALPHA_MOD (0.0)
#define MAX_ALPHA_MOD (1.0)

void main() {
	const vec3 rgb = vec3(1.0, 1.0, 1.0);

	float centre_dist = clamp(
		distance(vec2(0.5, 0.5), fs_in.texture_coords),
		MIN_CENTRE_DIST,
		MAX_CENTRE_DIST
	);

	float centre_distance_mod = SCALE(
		centre_dist,
		MIN_CENTRE_DIST,
		MAX_CENTRE_DIST,
		MAX_ALPHA_MOD,
		MIN_ALPHA_MOD
	);

	float noise_val = texture(noise_map, fs_in.texture_coords).a;

	float a = noise_val * centre_distance_mod;

	colour = vec4(rgb, a);
}
