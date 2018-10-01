#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "skybox.hpp"
#include "light_ball.hpp"
#include "terrain.hpp"
#include "shadow.hpp"
#include "cloud_scape.hpp"
#include "frame_buffer_view.hpp"

/// Manages lighting and Camera objects and rendering of objects in the scene
class Renderer {
public:
    Renderer(int window_width, int window_height);
    ~Renderer();
    void draw();
    void input_key(int key, int action);
    void check_input_frame(bool *keys);
    void set_window_size(int window_width, int window_height);
    void move_mouse(double mouse_x, double mouse_y);
    void cursor_lock(bool locked);
private:
	/// Options for draw mode
	enum Mode {
		DRAW_POINTS,
		DRAW_LINES,
		DRAW_FULL
	};
	
	/// Window size
	glm::vec2 m_window_size;
	/// Current draw mode
	Mode m_draw_mode;
	/// Delta frame render time
	GLfloat m_frame_delta_time;
	/// Last frame render time
	GLfloat m_last_frame_time;
	/// Vertex array object
	GLuint m_vertex_array_object;
	/// Textures
	Texture m_texture;
	/// Shaders
	Shader m_shader;
	/// Main Camera
	Camera m_camera;
	/// Shadows
	Shadow m_shadow;
	/// Frame buffer view
	FrameBufferView m_buffer_view;
	/// LightBall
	LightBall m_light_ball;
	/// Skybox
	Skybox m_skybox;
	/// Terrain
	Terrain m_terrain;
	/// Clouds
	CloudScape m_cloud_scape;
	/// A cube
	ObjectRender m_cube;
	/// A sphere
	ObjectRender m_sphere;
	/// Flag for whether to draw shadow map viewer
	GLboolean m_show_shadow_map_viewer;

	void store_frame_times();
	void set_depth_matrices();
	void shadow_pass();
	void update_depth_map();
	void draw_pass();
	void cycle_draw_mode();
};

#endif /* RENDERER_HPP */
