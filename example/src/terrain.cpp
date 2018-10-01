#include <stdio.h>
#include <ctime>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "noise/noise.h"

#include "terrain.hpp"
#include "object_render.hpp"
#include "noise_utils.hpp"
#include "shader.hpp"
#include "camera.hpp"

/// Initialise Terrain object

/// Create vertices, normals and indices for drawing Terrain
Terrain::Terrain(
	Shader *shader,
	Camera *camera,
	Texture *texture
) :
	ObjectRender(shader, camera),
	m_roughness(0.5f)
{
    printf("Generating terrain\n");

    GLuint texture_resolution = 15;

	GLfloat height_multiplier = 4.5f;

	GLuint x_size = 65;
	GLuint z_size = 65;
	GLuint resolution = 5;

	GLuint x_squares = (x_size * resolution);
	GLuint z_squares = (z_size * resolution);

	GLuint x_values = x_squares + 1;
	GLuint z_values = z_squares + 1;

    printf("...Generating noise\n");
	noise::utils::NoiseMap noise_map = create_noise_map(x_values, z_values);

	vertex_count = x_values * z_values;
	GLuint triangle_count = x_squares * z_squares * 2;
	index_count = triangle_count * 3;

	vertices = new glm::vec3[vertex_count];
	normals = new glm::vec3[vertex_count];
	texture_coords = new glm::vec2[vertex_count];
	indices = new GLuint[index_count];

	GLfloat x_inc = 1.0f / resolution;
	GLfloat z_inc = 1.0f / resolution;

	/* Set vertices */
	printf("...Initialising vertex and texture coord buffers\n");
	GLfloat tex_x_inc = static_cast<GLfloat>(texture_resolution) / x_values;
	GLfloat tex_y_inc = static_cast<GLfloat>(texture_resolution) / z_values;

	GLfloat tex_x = 0.0f;
	GLfloat x_pos = x_size / -2.0f;
	for(GLuint x=0; x<x_values; ++x, x_pos+=x_inc, tex_x+=tex_x_inc) {
		GLfloat tex_y = 0.0f;
		GLfloat z_pos = z_size / -2.0f;
		for(GLuint z=0; z<z_values; ++z, z_pos+=z_inc, tex_y+=tex_y_inc) {
			vertices[(x * x_values) + z] = glm::vec3(
				x_pos, 
				noise_map.GetValue(x, z) *  height_multiplier,
				z_pos
			);
			texture_coords[(x * x_values) + z] = glm::vec2(
				tex_x,
				tex_y
			);
		}
	}

	printf("...Initialising normal and index buffers\n");

	/* Initialise normals to zero */
	for(GLuint i=vertex_count; i--;) {
		normals[i] = glm::vec3(0.0f);
	}

	/* Set indices and calculate normals */
	GLuint indices_index = 0;
	for(GLuint x=0; x<x_squares; ++x) {
		GLuint top = x * x_values;
		GLuint bot = top + z_values;
		for(GLuint z=0; z<z_squares; ++z, ++top, ++bot) {
			GLuint triangle[2][3] = {
				{ top + 1, bot + 1, bot },
				{ top, top + 1, bot }
			};
			for(int i=2; i--;) {
				glm::vec3 normal = glm::cross(
					vertices[triangle[i][1]] - vertices[triangle[i][0]],
					vertices[triangle[i][2]] - vertices[triangle[i][0]]
				);
				for(int j=0; j<3; ++j, ++indices_index) {
					indices[indices_index] = triangle[i][j];
					normals[indices[indices_index]] += normal;
				}
			}
		}
	}

	/* Normalize normals */
	for(GLuint i=vertex_count; i--;) {
		normals[i] = glm::normalize(normals[i]);
	}

	init_vertices();

	/* Load textures */
	printf("...Loading textures\n");
	m_sand_texture = texture->load("textures/2D/sand.png");
	m_grass_texture = texture->load("textures/2D/grass.png");
	m_dark_grass_texture = texture->load("textures/2D/dark_grass.png");
	m_dirt_texture = texture->load("textures/2D/dirt.png");
	m_dark_dirt_texture = texture->load("textures/2D/dark_dirt.png");
	m_snow_texture = texture->load("textures/2D/snow.png");
}

/// Empty destructor

/// Storage is cleaned up in ObjectRender super-class
Terrain::~Terrain() {
	/* Empty */
}

/// Draw the Terrain object to frame buffer
void Terrain::draw(glm::vec3 light_pos) {
	glUseProgram(shader->oren_nayar.id);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_object);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

	glm::mat4 model = get_transform();
	glm::mat4 view = m_camera->view;
	glm::mat4 model_view = view * model;
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	glm::mat4 model_view_perspective = m_camera->transform * model;
	GLfloat camera_focal_length = m_camera->calc_focal_length();

	glUniformMatrix3fv(shader->oren_nayar.uniforms.normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
	glUniformMatrix4fv(shader->oren_nayar.uniforms.view, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(shader->oren_nayar.uniforms.model, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->oren_nayar.uniforms.model_view, 1, GL_FALSE, glm::value_ptr(model_view));
	glUniformMatrix4fv(shader->oren_nayar.uniforms.model_view_perspective, 1, GL_FALSE, glm::value_ptr(model_view_perspective));
	glUniform3fv(shader->oren_nayar.uniforms.camera_position, 1, glm::value_ptr(m_camera->position));
	glUniform1fv(shader->oren_nayar.uniforms.camera_focal_length, 1, &camera_focal_length);
	glUniform2fv(shader->oren_nayar.uniforms.window_size, 1, glm::value_ptr(m_camera->window_size));
	glUniform3fv(shader->oren_nayar.uniforms.light_position, 1, glm::value_ptr(light_pos));
	glUniform1fv(shader->oren_nayar.uniforms.roughness, 1, &m_roughness);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_sand_texture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_grass_texture);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_dark_grass_texture);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_dirt_texture);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_dark_dirt_texture);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_snow_texture);

	glDrawElements(
		GL_PATCHES,
		index_count,
		GL_UNSIGNED_INT,
		reinterpret_cast<GLvoid*>(0)
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

/// Draw the Terrain depth to buffer using shadow shader
void Terrain::draw_shadow() {
	glUseProgram(shader->shadow.id);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

	glm::mat4 model = get_transform();
	glUniformMatrix4fv(shader->shadow.uniforms.model, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(
		GL_TRIANGLES,
		index_count,
		GL_UNSIGNED_INT,
		reinterpret_cast<GLvoid*>(0)
	);

	glDisableVertexAttribArray(0);
}

/// Decrease terrain roughness
void Terrain::decrease_roughness() {
	m_roughness = glm::max(m_roughness - 0.1f, 0.0f);
}

/// Increase terrain roughness
void Terrain::increase_roughness() {
	m_roughness = glm::min(m_roughness + 0.1f, 1.0f);
}

/// Generate perlin noise map of the requested size

/// Modified from http://libnoise.sourceforge.net/tutorials/tutorial6.html
noise::utils::NoiseMap Terrain::create_noise_map(GLuint x_values, GLuint z_values) {
	int seed = time(NULL);

  	module::RidgedMulti mountain_terrain;

  	module::Billow base_flat_terrain;
	base_flat_terrain.SetFrequency(2.0);
	base_flat_terrain.SetPersistence(0.25f);
	base_flat_terrain.SetSeed(seed);

	module::Turbulence flat_terrain_turbulence;
	flat_terrain_turbulence.SetSourceModule(0, base_flat_terrain);
	flat_terrain_turbulence.SetFrequency(8.0);
	flat_terrain_turbulence.SetPower(0.075);
	flat_terrain_turbulence.SetSeed(seed);

	module::ScaleBias flat_terrain;
	flat_terrain.SetSourceModule(0, flat_terrain_turbulence);
	flat_terrain.SetScale(0.12);

	/* Controls how the terrain changes between types */
	/* Lower values mean larger areas of the same type */
	module::Perlin terrain_type;
	terrain_type.SetFrequency(0.75);
	terrain_type.SetPersistence(0.45);
	terrain_type.SetSeed(seed);

	module::Select terrain_selector;
	terrain_selector.SetSourceModule(0, flat_terrain);
	terrain_selector.SetSourceModule(1, mountain_terrain);
	terrain_selector.SetControlModule(terrain_type);
	terrain_selector.SetBounds(0.0, 1000.0);
	terrain_selector.SetEdgeFalloff(0.85);

	module::Turbulence final_terrain;
	final_terrain.SetSourceModule(0, terrain_selector);
	final_terrain.SetFrequency(0.8);
	final_terrain.SetPower(0.3);
	final_terrain.SetSeed(seed);

	utils::NoiseMap height_map;
	utils::NoiseMapBuilderPlane map_builder;
	map_builder.SetSourceModule(final_terrain);
	map_builder.SetDestNoiseMap(height_map);
	map_builder.SetDestSize(x_values, z_values);
	map_builder.SetBounds(5.0, 7.0, 1.0, 5.0);
	map_builder.Build();

	return height_map;
}
