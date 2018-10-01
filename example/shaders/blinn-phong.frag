/* Shadow mapping code modified from 
learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping */

#version 400

in VS_OUT {
    vec4 position;
    vec3 normal;
    vec2 texture_coords;
    vec4 shadow_position;
} fs_in;

out vec4 colour;

uniform mat4 view;
uniform mat4 model_view_perspective;
uniform vec3 light_position;
uniform vec3 emissive_light;
uniform float shininess;
uniform int textured;
uniform sampler2D tex;
uniform sampler2D shadow_texture;

const vec2 poisson_disk[4] = vec2[](
	vec2(-0.94201624, -0.39906216 ),
	vec2(0.94558609, -0.76890725 ),
	vec2(-0.094184101, -0.92938870 ),
	vec2(0.34495938, 0.29387760 )
);

void main() {
	const vec3 specular_albedo = vec3(1.0, 0.8, 0.6);

	colour = vec4(0.8, 0.8, 0.8, 0.8);
	if(textured != 0) {
		colour = texture(tex, fs_in.texture_coords);
	}

	vec3 light_direction = normalize(light_position);
	vec3 normal = normalize(fs_in.normal);

	float visibility = 1.0;
	if(fs_in.shadow_position.z <= 1.0) {
        //float bias = 0.005 * tan(acos(clamp(dot(normal, light_direction), 0, 1)));
		float bias = 0.005;
		for(int i=0; i<4; ++i){
			if(fs_in.shadow_position.z > texture(shadow_texture, fs_in.shadow_position.xy + poisson_disk[i]/3000.0 ).z + bias){
				visibility -= 0.25;
			}
		}
	}

	/* Ambient */
	vec3 ambient = colour.xyz * 0.1;

	/* Diffuse */
	vec3 diffuse = visibility * (clamp(dot(normal, light_direction), 0, 1) * colour.xyz);

	/* Specular */
	vec3 specular = vec3(0.0);
	if(dot(normal, light_direction) > 0) {
		vec3 V = normalize(-fs_in.position.xyz);
		vec3 half_dir = normalize(light_direction + V);
        specular = visibility * (pow(max(dot(normal, half_dir), 0.0), shininess) * specular_albedo.xyz);
	}

    colour = vec4(((ambient + diffuse) * colour.xyz) + specular + emissive_light, 1.0);
}
