#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"

/// Allows movement of a first person camera and provides view/projection matrices

/// Parts of class modified from: \n
/// http://learnopengl.com/#!Getting-started/Camera
class Camera {
private:
	/// Delta frame time pointer
	GLfloat *m_delta_time_ptr;
	/// Whether cursor is locked
    bool m_cursor_locked;
	/// Flag for first mouse input
	bool m_first_mouse_input;
	/// Mouse sensitivity setting
	GLfloat m_mouse_sensitivity;
	/// Last mouse x-coordinate
	double m_last_mouse_x;
	/// Last mouse y-coordinate
	double m_last_mouse_y;
	/// Movement speed
	GLfloat move_speed;
	/// Fast move speed modifier
	GLfloat fast_move_speed_mod;
	/// Field of view on Y-axis
	GLfloat fov_y;
	/// Close clipping distance
	GLfloat clip_close;
	/// Far clipping distance
	GLfloat clip_far;

	GLfloat calc_fov_x();
public:
	/// Size of the window
	glm::vec2 window_size;
	/// Aspect ratio
	GLfloat aspect_ratio;
	/// Position vector
	glm::vec3 position;
	/// Front vector
	glm::vec3 front;
	/// Up vector
	glm::vec3 up;
	/// Rotation yaw
	GLfloat yaw;
	/// Rotation pitch
	GLfloat pitch;
	/// Projection matrix
	glm::mat4 projection;
	/// View matrix
	glm::mat4 view;
	/// Transform (projection * view) matrix
	glm::mat4 transform;

	Camera(glm::vec2 window_size, GLfloat *delta_time_ptr);
	~Camera();
	void set_window_size(glm::vec2 window_size);
	void update();
	void move(bool forward, bool right, bool back, bool left, bool fast);
	void move_to(glm::vec3 position);
	void rotate(double mouse_x, double mouse_y);
    void cursor_lock(bool locked);
    GLfloat calc_focal_length();
    void set_pitch(GLfloat new_pitch);
};

#endif /* CAMERA */
