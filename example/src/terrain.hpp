#pragma once
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "noise/noise.h"

#include "object_render.hpp"
#include "noise_utils.hpp"
#include "shader.hpp"
#include "camera.hpp"

/// Handles creation and drawing of generated terrain
class Terrain : public ObjectRender {
public:
    Terrain(
		Shader *shader,
		Camera *camera,
		Texture *texture
	);
    ~Terrain();
    void draw(glm::vec3 light_pos);
    void draw_shadow();
    void decrease_roughness();
    void increase_roughness();
private:
	/// Terrain roughness
	GLfloat m_roughness;
	/// Sand texture id
	GLuint m_sand_texture;
	/// Grass texture id
	GLuint m_grass_texture;
	/// Dark grass texture id
	GLuint m_dark_grass_texture;
	/// Dirt texture id
	GLuint m_dirt_texture;
	/// Dark dirt texture id
	GLuint m_dark_dirt_texture;
	/// Snow texture id
	GLuint m_snow_texture;

	noise::utils::NoiseMap create_noise_map(GLuint x_values, GLuint z_values);
};

#endif /* TERRAIN_HPP */
