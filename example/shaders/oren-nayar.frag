/* Oren-Nayar diffuse lighting */
/* Modified from http://ruh.li/GraphicsOrenNayar.html */

#version 400

in TESE_OUT {
    vec4 position;
    vec3 normal;
    vec2 texture_coords;
    vec4 shadow_position;
    float height;
} frag_in;

out vec4 colour;

uniform mat4 model_view;
uniform float camera_focal_length;
uniform vec2 window_size;
uniform vec3 light_position;
uniform float roughness;
uniform sampler2D shadow_texture;
uniform sampler2D sand_texture;
uniform sampler2D grass_texture;
uniform sampler2D dark_grass_texture;
uniform sampler2D dirt_texture;
uniform sampler2D dark_dirt_texture;
uniform sampler2D snow_texture;

#define PI 3.141592653589793
#define PI_SQ (PI * PI)

const vec2 poisson_disk[4] = vec2[](
    vec2(-0.94201624, -0.39906216 ),
    vec2(0.94558609, -0.76890725 ),
    vec2(-0.094184101, -0.92938870 ),
    vec2(0.34495938, 0.29387760 )
);

void main() {
	const vec3 ambient = vec3(0.1, 0.1, 0.1);

	/* With help from stackoverflow.com/a/40596525/1791872 */
	vec3 view_direction = vec3(-(gl_FragCoord.xy - window_size * 0.5) / (window_size.x * 0.5), camera_focal_length);

	if(frag_in.height < -0.7) {
        colour = texture(sand_texture, frag_in.texture_coords);
    } else if(frag_in.height < -0.1) {
        colour = texture(grass_texture, frag_in.texture_coords);
	} else if(frag_in.height < 0.35) {
        colour = texture(dark_grass_texture, frag_in.texture_coords);
	} else if(frag_in.height < 1.15) {
        colour = texture(dirt_texture, frag_in.texture_coords);
    } else if(frag_in.height < 2.15) {
        colour = texture(dark_dirt_texture, frag_in.texture_coords);
	} else {
        colour = texture(snow_texture, frag_in.texture_coords);
	}

	vec3 normal = normalize(frag_in.normal);
	vec3 view_dir = normalize(view_direction);
	vec3 light_dir = normalize(light_position);

    float visibility = 1.0;
    if(frag_in.shadow_position.z <= 1.0) {
        /* Caused bad peter-panning and looks okay with constant bias */
        // float bias = 0.005 * tan(acos(clamp(dot(normal, light_dir), 0, 1)));

        float bias = 0.005;
        for(int i=0; i<4; ++i){
            if(frag_in.shadow_position.z > texture(shadow_texture, frag_in.shadow_position.xy + poisson_disk[i]/3000.0 ).z + bias){
                visibility -= 0.25;
            }
        }
    }

	float NdotL = dot(normal, light_dir);
    float NdotV = dot(normal, view_dir);

    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);
    
    float alpha = max(angleVN, angleLN);
    float beta = min(angleVN, angleLN);
    float gamma = dot(view_dir - normal * dot(view_dir, normal), light_dir - normal * dot(light_dir, normal));
    
    float roughness_sq = pow(roughness, 2);
    float roughness_sq_9 = roughness_sq / (roughness_sq + 0.09);
    float C1 = 1.0 - (0.5 * (roughness_sq / (roughness_sq + 0.33)));
    float C2 = 0.45 * roughness_sq_9;
    
    if(gamma > 0.0) {
        C2 *= sin(alpha);
    } else {
        C2 *= (sin(alpha) - pow((2.0 * beta) / PI, 3.0));
    }

    float powValue = (4.0 * alpha * beta) / PI_SQ;
    float C3  = 0.125 * roughness_sq_9 * powValue * powValue;

    float A = gamma * C2 * tan(beta);
    float B = (1.0 - abs(gamma)) * C3 * tan((alpha + beta) / 2.0);
    
    float L1 = max(0.0, NdotL) * (C1 + A + B);
    float two_beta_pi = 2.0 * beta / PI;
    float L2 = 0.17 * max(0.0, NdotL) * (roughness_sq / (roughness_sq + 0.13)) * (1.0 - gamma * two_beta_pi * two_beta_pi);

    colour = vec4((visibility * (colour.xyz * (L1 + L2))) + (ambient * colour.xyz), 1.0);
}
