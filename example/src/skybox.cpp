#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "skybox.hpp"

#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"

/// Initialise the Skybox object

/// Stores the pointers to Uniforms and Camera \n
/// Creates a list of vertices for the Skybox \n
/// Creates a buffer for the vertices \n
/// Loads the required cubemap texture and stores the ID
Skybox::Skybox(Shader *shader, Camera *camera, Texture *texture) :
	shader(shader),
	m_camera(camera)
{
    printf("Initialising skybox\n");

	glm::vec3 vertices[] = {
		glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
  
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
  
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
   
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
  
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
  
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(1.0f, -1.0f,  1.0f)
	};

	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	[0] = right
	[1] = left
	[2] = up
	[3] = down
	[4] = back
	[5] = front
	*/
	std::string textures[] = {
		"textures/skybox/thick_clouds/right.png",
		"textures/skybox/thick_clouds/left.png",
		"textures/skybox/thick_clouds/up.png",
		"textures/skybox/thick_clouds/down.png",
		"textures/skybox/thick_clouds/back.png",
		"textures/skybox/thick_clouds/front.png"
	};

	texture_id = texture->load_cubemap(textures);
}

/// Empty destructor
Skybox::~Skybox() {
	/* Empty */
}

/// Draw the skybox to frame buffer
void Skybox::draw() {
	glUseProgram(shader->skybox.id);
	
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glm::mat4 view_projection = m_camera->projection * glm::mat4(glm::mat3(m_camera->view));
    glUniformMatrix4fv(shader->skybox.uniforms.view_projection, 1, GL_FALSE, glm::value_ptr(view_projection));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

	glDisableVertexAttribArray(0);
}
