#pragma once
#ifndef LIGHT_BALL_HPP
#define LIGHT_BALL_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "shader.hpp"
#include "object_render.hpp"

namespace Light {
	/// Movement direction options
	enum Direction {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
}

/// Ball which emits light and can be moved around the scene
class LightBall : public ObjectRender {
public:
    LightBall(Shader *shader, Camera *camera, GLfloat *delta_time_ptr);
    ~LightBall();
	void move_to(glm::vec3 position);
	void move(Light::Direction direction);
	glm::vec3 camera_position();
	void draw();
private:
	/// Pointer to delta frame render time
	GLfloat *m_delta_time_ptr;
	/// Move speed
	GLfloat m_move_speed;
};

#endif /* LIGHT_BALL_HPP */
