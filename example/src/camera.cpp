#include <stdio.h>

#include "glm/glm.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/reciprocal.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.hpp"

/// Minimal constructor

/// Initialise class member variables
Camera::Camera(glm::vec2 window_size, GLfloat *delta_time_ptr) :
	m_delta_time_ptr(delta_time_ptr),
	m_cursor_locked(true),
	m_first_mouse_input(true),
	m_mouse_sensitivity(0.1f),
	move_speed(5.0f),
	fast_move_speed_mod(6.0),
	fov_y(75.0f),
	clip_close(0.001f),
	clip_far(250.0f),
	window_size(window_size),
	aspect_ratio(window_size.x/glm::max(window_size.y, 1.0f)),
	position(glm::vec3(0.0f, 0.0f, 0.0f)),
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	up(glm::vec3(0.0f, 1.0f,  0.0f)),
	yaw(-90.0f),
	pitch(0.0f),
	projection(glm::perspective(
		glm::radians(fov_y), 
		aspect_ratio,
		clip_close,
		clip_far
	)),
	view(glm::lookAt(position, position + front, up)),
	transform(projection * view)
{
	// Empty
}

/// Empty destructor
Camera::~Camera() {
	/* Empty */
}

/// Update perspective when window size changes
void Camera::set_window_size(glm::vec2 window_size) {
	this->window_size = window_size;
	aspect_ratio = this->window_size.x/glm::max(this->window_size.y, 1.0f);
	update();
}

/// Update transform matrices
void Camera::update() {
	projection = glm::perspective(
		glm::radians(fov_y), 
		aspect_ratio,
		clip_close,
		clip_far
	);
	view = glm::lookAt(position, position + front, up);
	transform = projection * view;
}

/// Move camera in a direction

/// Move speed controlled by speed class member variable \n
/// and controlled by delta frame render time
void Camera::move(bool forward, bool right, bool back, bool left, bool fast) {
	if(!(forward || right || back || left)) {
		return;
	}

	GLfloat speed = move_speed * (*m_delta_time_ptr);
	glm::vec3 change = glm::vec3(0.0f);

	if(forward) { change += front; }
	if(back) { change -= front; }
	if(right) { change += glm::normalize(glm::cross(front, up)); }
	if(left) { change -= glm::normalize(glm::cross(front, up)); }

	if(change == glm::vec3(0.0f)) {
		return;
	}
	
	//change.y = 0.0f;
	change = glm::normalize(change) * (fast ? speed * fast_move_speed_mod : speed);
	position += change;

	update();
}

/// Move camera to a specific location
void Camera::move_to(glm::vec3 position) {
	this->position = position;
	update();
}

/// Rotate camera to mouse coordinates
void Camera::rotate(double mouse_x, double mouse_y) {
	if(!m_cursor_locked) {
		return;
	}

	if(m_first_mouse_input) {
		m_last_mouse_x = mouse_x;
		m_last_mouse_y = mouse_y;
		m_first_mouse_input = false;
	}

	GLfloat x_delta = (mouse_x - m_last_mouse_x) * m_mouse_sensitivity;
    GLfloat y_delta = (m_last_mouse_y - mouse_y) * m_mouse_sensitivity;

    m_last_mouse_x = mouse_x;
    m_last_mouse_y = mouse_y;

    /* Set pitch and yaw */
    /* Limit yaw to 360 degrees */
    yaw = glm::mod(yaw + x_delta, 360.0f);
    pitch += y_delta;

    if(pitch > 89.0f) {
        pitch = 89.0f;
    }

    if(pitch < -89.0f) {
        pitch = -89.0f;
    }

    front = glm::normalize(glm::vec3(
    	glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
    	glm::sin(glm::radians(pitch)),
    	glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    ));

    update();
}

/// Lock cursor - stop camera movement
void Camera::cursor_lock(bool locked) {
	m_cursor_locked = locked;
	if(!m_cursor_locked) {
		m_first_mouse_input = true;
	}
}

/// Calculate focal length of the camera
GLfloat Camera::calc_focal_length() {
	return glm::cot(glm::radians(fov_y / 2.0f));
}

/// Calculate field of view on the X-axis
GLfloat Camera::calc_fov_x() {
	return glm::atan(glm::tan(glm::radians(0.5f * fov_y)) * aspect_ratio) * 2.0f;
}

/// Set camera pitch
void Camera::set_pitch(GLfloat new_pitch) {
	pitch = new_pitch;

	front = glm::normalize(glm::vec3(
    	glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
    	glm::sin(glm::radians(pitch)),
    	glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
    ));

    update();
}