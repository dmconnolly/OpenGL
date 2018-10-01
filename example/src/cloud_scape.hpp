#pragma once
#ifndef CLOUD_SCAPE_HPP
#define CLOUD_SCAPE_HPP

#include <chrono>
#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "cloud_layer.hpp"
#include "shader.hpp"
#include "random.hpp"
#include "camera.hpp"

#define CLOUD_LAYERS (4)

/// Used to create a number of clouds in the sky
class CloudScape {
public:
	CloudScape(Shader *shader, Camera *camera);
	~CloudScape();
	void draw();
private:
	/// Pointer to cloud shader
	Shader *m_shader;
	/// Pointer to camera
	Camera *m_camera;
	/// PCG random number generator wrapper
	Random m_rand;
	/// Array of cloud layer pointers
	CloudLayer *m_clouds[CLOUD_LAYERS];
};

#endif /* CLOUD_SCAPE_HPP */
