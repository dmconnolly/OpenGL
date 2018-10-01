#include <limits>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "cloud_scape.hpp"
#include "cloud_layer.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "random.hpp"

/// Generate noise and clouds
CloudScape::CloudScape(Shader *shader, Camera *camera) :
	m_shader(shader),
	m_camera(camera),
	m_rand()
{
	for(int i=0; i<CLOUD_LAYERS; ++i) {
        printf("Creating cloud layer %d/%d\n...", i+1, CLOUD_LAYERS);

		m_clouds[i] = new CloudLayer(
			m_shader, m_camera, i,
			m_rand.range(
				std::numeric_limits<int>::min(),
				std::numeric_limits<int>::max()
			)
		);
	}
}

/// Delete cloud layers
CloudScape::~CloudScape() {
	for(int i=CLOUD_LAYERS; i--;) {
		delete m_clouds[i];
	}
}

/// Draw clouds and update movement
void CloudScape::draw() {
	glUseProgram(m_shader->cloud.id);

	glDepthMask(GL_FALSE);

	/* Draw clouds */
	for(int i=CLOUD_LAYERS; i--;) {
		m_clouds[i]->draw();
	}

	glDepthMask(GL_TRUE);
}
