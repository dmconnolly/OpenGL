#version 400

layout(location = 0) in vec3 position;

uniform mat4 depth_matrix;
uniform mat4 model;

void main(){
	gl_Position = depth_matrix * model * vec4(position, 1.0);
}
