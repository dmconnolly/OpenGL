#include "GL/glew.h"
#include "glm/glm.hpp"

#include "light_ball.hpp"
#include "shader.hpp"
#include "object_render.hpp"

/// Initialise the LightBall instance

/// Call ObjectRender super-class constructor \n
/// Initialise class member variables
LightBall::LightBall(Shader *shader, Camera *camera, GLfloat *delta_time_ptr) :
	ObjectRender(shader, camera, "models/simple/", "sphere.obj"),
	m_delta_time_ptr(delta_time_ptr),
	m_move_speed(10.0f)
{
    printf("Initialising light\n");

	scale = glm::vec3(0.5f);
	emissive_light = glm::vec3(1.0f, 1.0f, 1.0f);
}

/// Empty destructor

/// Storage is cleaned up by ObjectRender super-class
LightBall::~LightBall() {
	/* Empty */
}

/// Move the LightBall in a direction

/// The speed of this movement is dependent on the class member \n
/// m_move_speed and is made uniform by use of the delta time pointer
void LightBall::move(Light::Direction direction) {
	GLfloat speed = m_move_speed * (*m_delta_time_ptr);
	glm::vec3 change = glm::vec3(0.0f);

	switch(direction) {
		case Light::LEFT:
			change.x -= speed;
			break;
		case Light::RIGHT:
			change.x += speed;
			break;
		case Light::UP:
			change.y += speed;
			break;
		case Light::DOWN:
			change.y -= speed;
			break;
		case Light::FORWARD:
			change.z -= speed;
			break;
		case Light::BACKWARD:
			change.z += speed;
			break;
	}
	translation += change;
}

/// Move the LightBall to a specific location
void LightBall::move_to(glm::vec3 position) {
	translation = position;
}

/// Get the current LightBall view position
glm::vec3 LightBall::camera_position() {
	return glm::vec3(m_camera->view * glm::vec4(translation, 0.0f));
}

/// Draw the LightBall
void LightBall::draw() {
	ObjectRender::draw(camera_position());
}
