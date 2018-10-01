#pragma once
#ifndef FRAME_BUFFER_VIEW_HPP
#define FRAME_BUFFER_VIEW_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"

/// Used to visualize a texture (shadow map)
class FrameBufferView {
public:
	/// Pointer to frame buffer view shader
	Shader *m_shader;
	/// Vertex buffer object id
	GLuint m_vertex_buffer_object;
	/// Texture coords buffer object id
	GLuint m_texture_coords_buffer_object;
	/// Index buffer object id
	GLuint m_index_buffer_object;

	/// Initialise frame buffer view

	/// Initialise vertex and index buffers
	FrameBufferView(Shader *shader) :
		m_shader(shader)
	{
		GLfloat vertices[] = {
			-1.0f, 1.0f, 0.0f,
			-0.44f, 1.0f, 0.0f,
			-0.44f, 0.50f, 0.0f,
			-1.0f, 0.50f, 0.0f
		};

		GLfloat texture_coords[] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, -1.0f,
			-1.0f, -1.0f,
		};

		GLuint indices[] = {
			3, 2, 1, 3, 1, 0
		};

		glGenBuffers(1, &m_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_texture_coords_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, m_texture_coords_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);

		glGenBuffers(1, &m_index_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	/// Empty destructor
	~FrameBufferView() {
		/* Empty */
	}

	/// Draw the frame buffer view
	void draw() {
		glUseProgram(m_shader->debug_frame_buffer.id);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_texture_coords_buffer_object);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_object);

		glDrawElements(
			GL_TRIANGLES,
			6, 
			GL_UNSIGNED_INT, 
			reinterpret_cast<GLvoid*>(0)
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
};

#endif /* FRAME_BUFFER_VIEW_HPP */
