#pragma once
#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"

/// Handles loading and drawing of a skybox (cubemap)
class Skybox {
public:
    Skybox(Shader *shader, Camera *camera, Texture *texture);
    ~Skybox();
    void draw();
private:
	/// Pointer to the object shader
	Shader *shader;
	/// Pointer to the Camera object
	Camera *m_camera;
	/// ID for the cubemap texture
	GLuint texture_id;
	/// Vertex buffer
	GLuint vertex_buffer_object;
};

#endif /* SKYBOX_HPP */
