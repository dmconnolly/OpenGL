#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "cloud_layer.hpp"
#include "object_render.hpp"
#include "fast_noise.hpp"

#define SIZE_X (180)
#define SIZE_Z (180)
#define NOISE_RESOLUTION (5)

#define NOISE_X (SIZE_X * NOISE_RESOLUTION)
#define NOISE_Y (SIZE_Z * NOISE_RESOLUTION)

#define SCALE(x,a1,a2,b1,b2) ((((b2-b1)*(x-a1))/(a2-a1))+b1)

/// Initialise the cloud layer

/// Initialise buffers and generate noise
CloudLayer::CloudLayer(
	Shader *shader,
	Camera *camera,
	GLuint level,
	int seed
) :
	ObjectRender(shader, camera),
	m_level(level),
	m_seed(seed)
{
	set_level_values();

	printf("...Initialising buffers\n");

	vertex_count = 4;
	index_count = 6;

	vertices = new glm::vec3[4] {
		glm::vec3(-SIZE_X, m_height, SIZE_Z),
		glm::vec3(SIZE_X, m_height, SIZE_Z),
		glm::vec3(SIZE_X, m_height, -SIZE_Z),
		glm::vec3(-SIZE_X, m_height, -SIZE_Z)
	};

	texture_coords = new glm::vec2[4] {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
	};

	indices = new GLuint[6] {
		3, 2, 1, 3, 1, 0
	};

	init_vertices();

	generate_noise_map();
}

/// Empty destructor
CloudLayer::~CloudLayer() {
	/* Empty */
}

/// Draw the clouds
void CloudLayer::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_object);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

	glm::mat4 model = get_transform();
	glm::mat4 model_view = m_camera->view * model;
	glm::mat4 model_view_perspective = m_camera->transform * model;

	glUniformMatrix4fv(shader->cloud.uniforms.model, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->cloud.uniforms.model_view, 1, GL_FALSE, glm::value_ptr(model_view));
	glUniformMatrix4fv(shader->cloud.uniforms.model_view_perspective, 1, GL_FALSE, glm::value_ptr(model_view_perspective));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_noise_map_id);

	glDrawElements(
		GL_TRIANGLES,
		index_count, 
		GL_UNSIGNED_INT, 
		reinterpret_cast<GLvoid*>(0)
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

/// Set class member variables based on current cloud level
void CloudLayer::set_level_values() {
	switch(m_level) {
		case 0:
			m_height = 30.0f;
			m_alpha_bias = -1.25f;
			break;
		case 1:
			m_height = 60.0f;
			m_alpha_bias = -1.0f;
			break;
		case 2:
			m_height = 90.0f;
			m_alpha_bias = -1.0f;
			break;
		case 3:
			m_height = 120.0f;
			m_alpha_bias = -0.5f;
			break;
	}
}

/// Generate noise map for cloud layer
void CloudLayer::generate_noise_map() {
	printf("Generating noise\n");

	GLubyte *noise_map = new GLubyte[NOISE_X * NOISE_Y];

	FastNoise noise;
	noise.SetSeed(m_seed);
	noise.SetFrequency(0.01f);
	noise.SetNoiseType(FastNoise::ValueFractal);
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(2.0f);
	noise.SetFractalGain(0.5f);
	noise.SetFractalType(FastNoise::FBM);

	for(GLuint x=0; x<NOISE_X; ++x) {
		for(GLuint y=0; y<NOISE_Y; ++y) {
			GLfloat noise_val = glm::clamp(noise.GetNoise(x, y) + m_alpha_bias, -1.0f, 1.0f);
			GLuint noise_index = (NOISE_X * y) + x;
			noise_map[noise_index] = static_cast<GLubyte>(
				SCALE(noise_val, -1.0f, 1.0f, 0.0f, 255.0f)
			);
		}
	}

	glGenTextures(1, &m_noise_map_id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_noise_map_id);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_ALPHA,
		NOISE_X, NOISE_Y, 0, GL_ALPHA,
		GL_UNSIGNED_BYTE, noise_map
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] noise_map;
}
