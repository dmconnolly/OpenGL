#pragma once
#ifndef OBJECT_RENDER_HPP
#define OBJECT_RENDER_HPP

#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"

/// Generic object for rendering from .obj file
class ObjectRender {
public:
	/// Pointer to object shader
	Shader *shader;
	/// Scale vector
	glm::vec3 scale;
	/// Translation vector
	glm::vec3 translation;
	/// Global rotation vector
	glm::vec3 global_rotation;
	/// Global rotation speed vector
	glm::vec3 global_rotation_speed;
	/// Local rotation vector
	glm::vec3 local_rotation;
	/// Local rotation speed vector
	glm::vec3 local_rotation_speed;
	/// Emissive light component vector
	glm::vec3 emissive_light;
	/// Object shininess
	GLfloat shininess;
	/// ID of object texture
	GLuint texture_id;

	ObjectRender(
		Shader *shader,
		Camera *camera,
		Texture *texture,
		const char *model_file_path,
		const char *obj_file_name
	);
	ObjectRender(
		Shader *shader,
		Camera *camera,
		const char *model_file_path,
		const char *obj_file_name
	);
    ~ObjectRender();
	void draw(glm::vec3 light_pos);
	void draw_shadow();
protected:
	/// Camera instance pointer
	Camera *m_camera;
	/// Number of vertices
	std::size_t vertex_count;
	/// Number of indices
	std::size_t index_count;
	/// Whether the object has texture coordinates
	GLint uv_mapped;
	/// Whether the object is textured
	GLint textured;
	/// Pointer to vertex positions
    glm::vec3 *vertices;
    /// Pointer to vertex normals
	glm::vec3 *normals;
	/// Pointer to vertex texture coordinates
	glm::vec2 *texture_coords;
	/// Pointer to vertex colours
	glm::vec4 *colours;
	/// Pointer to indices into vertex attribute arrays
	GLuint *indices;
	/// Vertex buffer object
	GLuint vertex_buffer_object;
	/// Normal buffer object
	GLuint normal_buffer_object;
	/// texture coordinates buffer object
	GLuint texture_coord_buffer_object;
	/// Vertex colours buffer object
	GLuint colour_buffer_object;
	/// Index buffer object
	GLuint index_buffer_object;

	ObjectRender(Shader *shader, Camera *camera);
	void init_vertices();
    glm::mat4 get_transform();

private:
	GLfloat modulo_degrees(GLfloat degrees);
};

#endif /* OBJECT_RENDER_HPP */
