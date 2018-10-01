#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

#include "renderer.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "skybox.hpp"
#include "light_ball.hpp"
#include "cloud_scape.hpp"
#include "terrain.hpp"
#include "shadow.hpp"

/// Initialise the Renderer

/// Initialise class member variables \n
/// Create and bind Vertex Array Object (VAO) \n
/// Load and initialise shaders \n
/// Create and initialise objects in the scene
Renderer::Renderer(int window_width, int window_height) :
	m_window_size(glm::vec2(
		static_cast<GLfloat>(window_width),
		static_cast<GLfloat>(window_height)
	)),
    m_draw_mode(DRAW_FULL),
	m_frame_delta_time(0.0f),
	m_last_frame_time(0.0f),
	m_texture(),
	m_shader(),
	m_camera(m_window_size, &m_frame_delta_time),
	m_shadow(&m_shader),
	m_buffer_view(&m_shader),
	m_light_ball(&m_shader, &m_camera, &m_frame_delta_time),
	m_skybox(&m_shader, &m_camera, &m_texture),
	m_terrain(&m_shader, &m_camera, &m_texture),
	m_cloud_scape(&m_shader, &m_camera),
	m_cube(&m_shader, &m_camera, "models/simple/", "cube.obj"),
	m_sphere(&m_shader, &m_camera, "models/simple/", "sphere.obj"),
	m_show_shadow_map_viewer(false)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Generate index for a vertex array object */
	glGenVertexArrays(1, &m_vertex_array_object);

	/* Create the vertex array object and make it current */
	glBindVertexArray(m_vertex_array_object);

	/* Initialise objects in the scene */
    printf("\nMoving objects\n");

    m_camera.set_pitch(5.0f);
	m_camera.move_to(glm::vec3(0.0f, 10.0f, 38.0f));

	m_light_ball.move_to(glm::vec3(-10.0f, 25.0f, 10.0f));

	m_cube.translation = glm::vec3(-2.5f, 17.0f, -2.5f);
	m_cube.scale = glm::vec3(1.5f);

	m_sphere.translation = glm::vec3(0.0f, 10.0f, 0.0f);
	m_sphere.scale = glm::vec3(1.5f);
}

/// Minimal destructor

/// Empty destructor
Renderer::~Renderer() {
	/* Empty */
}

/// Draw objects in the scene

/// Store last frame time and calculate delta frame time \n
/// Clear the scene background and buffers \n
/// Draw objects in the scene to the frame buffer
void Renderer::draw() {
	store_frame_times();
	set_depth_matrices();
	shadow_pass();
	update_depth_map();
	draw_pass();
}

/// Handle key presses when an event is triggered

/// This method is not called every frame, but only when \n
/// the key is pressed, release or repeated. This means that \n
/// there will be delays if a key is held down. This makes this \n
/// method ideal for controls such as toggling options.
void Renderer::input_key(int key, int action) {
	if(action != GLFW_PRESS && action != GLFW_REPEAT) {
		return;
	}

    if(key == GLFW_KEY_TAB) { cycle_draw_mode(); }

    if(key == GLFW_KEY_Q) { m_terrain.decrease_roughness(); }
    if(key == GLFW_KEY_E) { m_terrain.increase_roughness(); }

    if(key == GLFW_KEY_R) { m_show_shadow_map_viewer = !m_show_shadow_map_viewer; }
}

/// Handle key presses on a per-frame basis

/// This method is called every frame and takes an array of booleans \n
/// as a parameter, each bool represents whether a specific key is \n
/// currently pressed. As a result, this function allows actions to take \n
/// place every frame. This is ideal for smooth movement controls such as \n
/// moving the camera or light source.\n\n
/// Movement triggered through this function should use the frame delta time \n
/// to ensure that movement speed is the same across different platforms.
void Renderer::check_input_frame(bool *keys) {
	m_camera.move(
		keys[GLFW_KEY_W],
		keys[GLFW_KEY_D],
		keys[GLFW_KEY_S],
		keys[GLFW_KEY_A],
		keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]
	);

    if(keys[GLFW_KEY_KP_8]) { m_light_ball.move(Light::FORWARD); }
    if(keys[GLFW_KEY_KP_5]) { m_light_ball.move(Light::BACKWARD); }
    if(keys[GLFW_KEY_KP_4]) { m_light_ball.move(Light::LEFT); }
    if(keys[GLFW_KEY_KP_6]) { m_light_ball.move(Light::RIGHT); }
    if(keys[GLFW_KEY_KP_7]) { m_light_ball.move(Light::DOWN); }
    if(keys[GLFW_KEY_KP_9]) { m_light_ball.move(Light::UP); }
}

/// Callback for window resize
void Renderer::set_window_size(int window_width, int window_height) {
	m_window_size = glm::vec2(
		static_cast<GLfloat>(window_width),
		static_cast<GLfloat>(window_height)
	);
	m_camera.set_window_size(m_window_size);
}

/// Callback for mouse movement
void Renderer::move_mouse(double mouse_x, double mouse_y) {
	m_camera.rotate(mouse_x, mouse_y);
}

/// Callback for cursor lock
void Renderer::cursor_lock(bool locked) {
	m_camera.cursor_lock(locked);
}

/// Store last frame render time and frame delta time
void Renderer::store_frame_times() {
	GLfloat current_frame_time = glfwGetTime();
	m_frame_delta_time = current_frame_time - m_last_frame_time;
	m_last_frame_time = current_frame_time;
}

/// Calculate depth matrices and send to shaders
void Renderer::set_depth_matrices() {
	glm::mat4 light_depth_matrix = m_shadow.get_depth_matrix(m_light_ball.translation);
	glUseProgram(m_shader.blinn_phong.id);
	glUniformMatrix4fv(m_shader.blinn_phong.uniforms.depth_matrix, 1, GL_FALSE, glm::value_ptr(light_depth_matrix));
	glUseProgram(m_shader.oren_nayar.id);
	glUniformMatrix4fv(m_shader.oren_nayar.uniforms.depth_matrix, 1, GL_FALSE, glm::value_ptr(light_depth_matrix));
}

/// First render pass to draw depth map texture
void Renderer::shadow_pass() {
	glViewport(0, 0, m_shadow.texture_size.x, m_shadow.texture_size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadow.frame_buffer_id);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);
	m_cube.draw_shadow();
	m_sphere.draw_shadow();
	m_terrain.draw_shadow();
}

/// Update depth map in textures
void Renderer::update_depth_map() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadow.depth_texture_id);
}

/// Second render pass to draw objects in the scene
void Renderer::draw_pass() {
	glViewport(0, 0, m_window_size.x, m_window_size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_BACK);
	if(m_show_shadow_map_viewer) {
		m_buffer_view.draw();
	}
	m_terrain.draw(m_light_ball.camera_position());
	m_cube.draw(m_light_ball.camera_position());
	m_sphere.draw(m_light_ball.camera_position());
	m_light_ball.draw();
	m_skybox.draw();
	m_cloud_scape.draw();
}

/// Callback to change current draw mode
void Renderer::cycle_draw_mode() {
    m_draw_mode = static_cast<Mode>((m_draw_mode + 1) % 3);
	switch(m_draw_mode) {
		case DRAW_POINTS:
			glPointSize(3.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		case DRAW_LINES:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case DRAW_FULL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
	}
}
