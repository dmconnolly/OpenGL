#pragma once
#ifndef CLOUD_PLANE_HPP
#define CLOUD_PLANE_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "shader.hpp"
#include "object_render.hpp"

/// Class for generating a layer of clouds
class CloudLayer : public ObjectRender {
public:
	CloudLayer(
		Shader *shader,
		Camera *camera,
		GLuint level,
		int seed
	);
	~CloudLayer();
	void draw();
private:
	/// Cloud level to generate
	GLuint m_level;
	/// Seed for noise generation
	int m_seed;
	/// Cloud layer height
	GLfloat m_height;
	/// Current bias for generated noise levels
	GLfloat m_alpha_bias;
	/// Noise texture id
	GLuint m_noise_map_id;

	void set_level_values();
	void generate_noise_map();
};

#endif /* CLOUD_PLANE_HPP */
