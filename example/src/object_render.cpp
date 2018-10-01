#include <iostream>
#include <string>
#include <tuple>
#include <map>
#include <stdio.h>

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj/tiny_obj_loader.h"

#include "object_render.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"

/// Basic constructor

/// Initialise class member variables
ObjectRender::ObjectRender(Shader *shader, Camera *camera) :
	shader(shader),
	scale(glm::vec3(1.0f)),
	translation(glm::vec3(0.0f)),
	global_rotation(glm::vec3(0.0f)),
	global_rotation_speed(glm::vec3(0.0f)),
	local_rotation(glm::vec3(0.0f)),
	local_rotation_speed(glm::vec3(0.0f)),
	emissive_light(glm::vec3(0.0f)),
	shininess(32.0f),
	m_camera(camera),
	vertex_count(0),
	index_count(0),
	textured(0),
	vertices(NULL),
	normals(NULL),
	texture_coords(NULL),
	colours(NULL),
	indices(NULL)
{
	/* Empty */
}

/// Full constructor

/// Load .obj file and .mtl file for object \n
/// Vertices, normals and texture coordinates are parsed and formatted \n
/// in a way that makes it possible to draw directly through OpenGL. \n
/// OpenGL only allows indexing of vertices, and not separately for normals \n
/// and texture coordinates. It is for this reason that a map is used in order \n
/// to create duplicate vertices where required for drawing.
ObjectRender::ObjectRender(
		Shader *shader,
		Camera *camera,
		Texture *texture,
		const char *model_file_path,
		const char *obj_file_name) :
	ObjectRender(shader, camera)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	char *obj_file = (char*)malloc(
		(strlen(model_file_path) + 
		strlen(obj_file_name) + 1) * sizeof(char)
	);

	if(!obj_file) {
		printf("Obj file name malloc failed\n");
	}
	memcpy(
		obj_file,
		model_file_path,
		strlen(model_file_path)
	);
    memcpy(
    	obj_file + strlen(model_file_path),
    	obj_file_name,
    	strlen(obj_file_name) + 1
    );

	std::string load_error;
	bool load_success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&materials,
		&load_error,
		obj_file,
		model_file_path
	);

	free(obj_file);

	tinyobj::mesh_t mesh = shapes[0].mesh;

	if(!load_error.empty()) {
		std::cerr << load_error << std::endl;
	}

	if(!load_success) {
		printf("Failed to load/parse .obj.\n");
		return;
	}

	uv_mapped = attributes.texcoords.size() > 0;
	textured = texture && uv_mapped;

	std::vector<GLuint> indices_vector;
	std::map<std::tuple<GLuint, GLuint, GLuint>, GLuint> vertex_indices;
	std::vector<std::tuple<GLuint, GLuint, GLuint>> vertex_indices_tuple_vector;

	for(GLuint i=0; i<mesh.indices.size(); ++i) {
		GLuint vertex_index = mesh.indices[i].vertex_index;
		GLuint normal_index = mesh.indices[i].normal_index;
		GLuint texture_coords_index = uv_mapped ? mesh.indices[i].texcoord_index : 0;

		std::tuple<GLuint, GLuint, GLuint> vertex_indices_tuple = std::make_tuple(
			vertex_index,
			normal_index,
			texture_coords_index
		);

		std::map<std::tuple<GLuint, GLuint, GLuint>, GLuint>::iterator it = vertex_indices.find(
			vertex_indices_tuple
		);

		if(it == vertex_indices.end()) {
			vertex_indices[vertex_indices_tuple] = vertex_count;
			vertex_indices_tuple_vector.push_back(vertex_indices_tuple);
			indices_vector.push_back(vertex_count);
		    ++vertex_count;
		} else {
			indices_vector.push_back(it->second);
		}
	}

	index_count = indices_vector.size();

	vertices = new glm::vec3[vertex_count];
	normals = new glm::vec3[vertex_count];
	texture_coords = new glm::vec2[vertex_count];
	indices = new GLuint[index_count];

	for(GLuint i=0; i<vertex_count; ++i) {
		GLuint vertex_index = std::get<0>(vertex_indices_tuple_vector[i]);
		GLuint normal_index = std::get<1>(vertex_indices_tuple_vector[i]);
		GLuint texture_coords_index = std::get<2>(vertex_indices_tuple_vector[i]);

		memcpy(&vertices[i], &attributes.vertices[vertex_index*3], sizeof(glm::vec3));
		memcpy(&normals[i], &attributes.normals[normal_index*3], sizeof(glm::vec3));

		if(uv_mapped) {
			memcpy(
				&texture_coords[i],
				&attributes.texcoords[texture_coords_index*2],
				sizeof(glm::vec2)
			);
		}
	}

	for(GLuint i=0; i<index_count; ++i) {
		indices[i] = indices_vector[i];
	}

	init_vertices();

	/* Load diffuse texture */
	if(textured) {
		texture_id = texture->load(materials[0].diffuse_texname.c_str());
	}
}

/// Intermediary constructor for when texturing is not required

/// This is useful during debugging or creation of a new object
ObjectRender::ObjectRender(
		Shader *shader,
		Camera *camera,
		const char *model_file_path,
		const char *obj_file_name) :
	ObjectRender(shader, camera, NULL, model_file_path, obj_file_name)
{
	/* Empty */
}

/// Minimal destructor

/// Delete vertex attribute storage objects
ObjectRender::~ObjectRender() {
    delete[] vertices;
	delete[] normals;
	delete[] texture_coords;
	delete[] colours;
	delete[] indices;
}

/// Returns latest matrix transform for the object
glm::mat4 ObjectRender::get_transform() {
	glm::vec3 global_rotate_radians = glm::vec3(
		glm::radians(modulo_degrees(global_rotation.x)),
		glm::radians(modulo_degrees(global_rotation.y)),
		glm::radians(modulo_degrees(global_rotation.z))
	);
	glm::vec3 local_rotate_radians = glm::vec3(
		glm::radians(modulo_degrees(local_rotation.x)),
		glm::radians(modulo_degrees(local_rotation.y)),
		glm::radians(modulo_degrees(local_rotation.z))
	);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, global_rotate_radians.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, global_rotate_radians.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, -global_rotate_radians.z, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, translation);
	transform = glm::rotate(transform, local_rotate_radians.x, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, local_rotate_radians.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, -local_rotate_radians.z, glm::vec3(0, 0, 1));
	transform = glm::scale(transform, scale);

	return transform;
}

/// Initialise vertex attribute buffers
void ObjectRender::init_vertices() {
	std::size_t buffer_size;

	buffer_size = vertex_count * sizeof(glm::vec3);
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, vertices, GL_STATIC_DRAW);

	if(normals) {
		buffer_size = vertex_count * sizeof(glm::vec3);
		glGenBuffers(1, &normal_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, normals, GL_STATIC_DRAW);
	}

	if(texture_coords) {
		buffer_size = vertex_count * sizeof(glm::vec2);
		glGenBuffers(1, &texture_coord_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, texture_coords, GL_STATIC_DRAW);
	}

	if(colours) {
		buffer_size = vertex_count * sizeof(glm::vec4);
		glGenBuffers(1, &texture_coord_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, colour_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, colours, GL_STATIC_DRAW);
	}

	if(indices) {
		buffer_size = index_count * sizeof(GLuint);
		glGenBuffers(1, &index_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, indices, GL_STATIC_DRAW);
	}
}

/// Draw the object to frame buffer
void ObjectRender::draw(glm::vec3 light_pos) {
	glUseProgram(shader->blinn_phong.id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texture_coord_buffer_object);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

	glm::mat4 model = get_transform();
	glm::mat4 view = m_camera->view;
	glm::mat4 model_view = view * model;
	glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_view)));
	glm::mat4 model_view_perspective = m_camera->transform * model;

	glUniformMatrix3fv(shader->blinn_phong.uniforms.normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
	glUniformMatrix4fv(shader->blinn_phong.uniforms.view, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(shader->blinn_phong.uniforms.model, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->blinn_phong.uniforms.model_view, 1, GL_FALSE, glm::value_ptr(model_view));
	glUniformMatrix4fv(shader->blinn_phong.uniforms.model_view_perspective, 1, GL_FALSE, glm::value_ptr(model_view_perspective));
	glUniform3fv(shader->blinn_phong.uniforms.light_position, 1, glm::value_ptr(light_pos));
	glUniform3fv(shader->blinn_phong.uniforms.emissive_light, 1, glm::value_ptr(emissive_light));
	glUniform1fv(shader->blinn_phong.uniforms.shininess, 1, &shininess);
	glUniform1iv(shader->blinn_phong.uniforms.textured, 1, &textured);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDrawElements(
		GL_TRIANGLES,
		index_count, 
		GL_UNSIGNED_INT, 
		reinterpret_cast<GLvoid*>(0)
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

/// Draw the object's depth to buffer using shadow shader
void ObjectRender::draw_shadow() {
	glUseProgram(shader->shadow.id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);

	glm::mat4 model = get_transform();
	glUniformMatrix4fv(shader->shadow.uniforms.model, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(
		GL_TRIANGLES,
		index_count, 
		GL_UNSIGNED_INT, 
		reinterpret_cast<GLvoid*>(0)
	);

	glDisableVertexAttribArray(0);
}

/// Get a value of degress between -359 and 359
GLfloat ObjectRender::modulo_degrees(GLfloat degrees) {
	while(degrees >= 360.0f) {
		degrees -= 360.0f;
	}
	while(degrees <= -360.0f) {
		degrees += 360.0f;
	}
	return degrees;
}
