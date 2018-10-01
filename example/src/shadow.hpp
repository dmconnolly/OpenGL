#pragma once
#ifndef SHADOW_HPP
#define SHADOW_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "shader.hpp"
#include "camera.hpp"

/// Functionality for shadow casting

/// Implemented using the following guides:-\n
/// learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
/// opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping
class Shadow {
public:
	/// ID of frame buffer
    GLuint frame_buffer_id;
    /// ID of depth texture
    GLuint depth_texture_id;
    /// Dimensions of depth map texture
	glm::vec2 texture_size;

	Shadow(Shader *shader);
	~Shadow();
	glm::mat4 get_depth_matrix(glm::vec3 light_position);
private:
	/// Pointer to shadow shader
	Shader *m_shader;
};

#endif /* SHADOW_HPP */
