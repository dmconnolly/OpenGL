#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "GL/glew.h"

#include "shader.hpp"

/// Minimal constructor

/// Initialise shaders
Shader::Shader() {
    printf("Loading shaders\n");

	/* Blinn-Phong */
	printf("...Object shader (Blinn-Phong)\n");
	blinn_phong.id = Shader::load(
		"shaders/blinn-phong.vert",
		NULL,
		NULL,
		NULL,
		"shaders/blinn-phong.frag"
	);
	glUseProgram(blinn_phong.id);
	blinn_phong.uniforms.normal_matrix = glGetUniformLocation(blinn_phong.id, "normal_matrix");
	blinn_phong.uniforms.view = glGetUniformLocation(blinn_phong.id, "view");
	blinn_phong.uniforms.model = glGetUniformLocation(blinn_phong.id, "model");
	blinn_phong.uniforms.model_view = glGetUniformLocation(blinn_phong.id, "model_view");
	blinn_phong.uniforms.model_view_perspective = glGetUniformLocation(blinn_phong.id, "model_view_perspective");
	blinn_phong.uniforms.light_position = glGetUniformLocation(blinn_phong.id, "light_position");
	blinn_phong.uniforms.emissive_light = glGetUniformLocation(blinn_phong.id, "emissive_light");
	blinn_phong.uniforms.shininess = glGetUniformLocation(blinn_phong.id, "shininess");
	blinn_phong.uniforms.textured = glGetUniformLocation(blinn_phong.id, "textured");
	blinn_phong.uniforms.texture = glGetUniformLocation(blinn_phong.id, "tex");
	blinn_phong.uniforms.depth_matrix = glGetUniformLocation(blinn_phong.id, "depth_matrix");
	blinn_phong.uniforms.shadow_texture = glGetUniformLocation(blinn_phong.id, "shadow_texture");
	glUniform1i(blinn_phong.uniforms.texture, 0);
	glUniform1i(blinn_phong.uniforms.shadow_texture, 1);

	/* Oren Nayar */
	printf("...Terrain shader (Oren-Nayar)\n");
	oren_nayar.id = Shader::load(
		"shaders/oren-nayar.vert",
		"shaders/oren-nayar.tesc",
		"shaders/oren-nayar.tese",
		NULL,
		"shaders/oren-nayar.frag"
	);
	glUseProgram(oren_nayar.id);
	oren_nayar.uniforms.view = glGetUniformLocation(oren_nayar.id, "view");
	oren_nayar.uniforms.model = glGetUniformLocation(oren_nayar.id, "model");
	oren_nayar.uniforms.model_view = glGetUniformLocation(oren_nayar.id, "model_view");
	oren_nayar.uniforms.model_view_perspective = glGetUniformLocation(oren_nayar.id, "model_view_perspective");
	oren_nayar.uniforms.normal_matrix = glGetUniformLocation(oren_nayar.id, "normal_matrix");
	oren_nayar.uniforms.camera_position = glGetUniformLocation(oren_nayar.id, "camera_position");
	oren_nayar.uniforms.camera_focal_length = glGetUniformLocation(oren_nayar.id, "camera_focal_length");
	oren_nayar.uniforms.light_position = glGetUniformLocation(oren_nayar.id, "light_position");
	oren_nayar.uniforms.depth_matrix = glGetUniformLocation(oren_nayar.id, "depth_matrix");
	oren_nayar.uniforms.shadow_texture = glGetUniformLocation(oren_nayar.id, "shadow_texture");
	oren_nayar.uniforms.window_size = glGetUniformLocation(oren_nayar.id, "window_size");
	oren_nayar.uniforms.roughness = glGetUniformLocation(oren_nayar.id, "roughness");
	oren_nayar.uniforms.shadow_texture = glGetUniformLocation(oren_nayar.id, "shadow_texture");
	oren_nayar.uniforms.sand_texture = glGetUniformLocation(oren_nayar.id, "sand_texture");
	oren_nayar.uniforms.grass_texture = glGetUniformLocation(oren_nayar.id, "grass_texture");
	oren_nayar.uniforms.dark_grass_texture = glGetUniformLocation(oren_nayar.id, "dark_grass_texture");
	oren_nayar.uniforms.dirt_texture = glGetUniformLocation(oren_nayar.id, "dirt_texture");
	oren_nayar.uniforms.dark_dirt_texture = glGetUniformLocation(oren_nayar.id, "dark_dirt_texture");
	oren_nayar.uniforms.snow_texture = glGetUniformLocation(oren_nayar.id, "snow_texture");
	glUniform1i(oren_nayar.uniforms.shadow_texture, 1);
	glUniform1i(oren_nayar.uniforms.sand_texture, 3);
	glUniform1i(oren_nayar.uniforms.grass_texture, 4);
	glUniform1i(oren_nayar.uniforms.dark_grass_texture, 5);
	glUniform1i(oren_nayar.uniforms.dirt_texture, 6);
	glUniform1i(oren_nayar.uniforms.dark_dirt_texture, 7);
	glUniform1i(oren_nayar.uniforms.snow_texture, 8);

	/* Skybox */
	printf("...Skybox shader\n");
	skybox.id = Shader::load(
		"shaders/skybox.vert",
		NULL,
		NULL,
		NULL,
		"shaders/skybox.frag"
	);
	glUseProgram(skybox.id);
	skybox.uniforms.view_projection = glGetUniformLocation(skybox.id, "view_projection");
	skybox.uniforms.texture = glGetUniformLocation(skybox.id, "tex");
	glUniform1i(skybox.uniforms.texture, 0);

	/* Shadow */
	printf("...Shadow shader\n");
	shadow.id = Shader::load(
		"shaders/shadow.vert",
		NULL,
		NULL,
		NULL,
		"shaders/shadow.frag"
	);
	glUseProgram(shadow.id);
	shadow.uniforms.model = glGetUniformLocation(shadow.id, "model");
	shadow.uniforms.depth_matrix = glGetUniformLocation(shadow.id, "depth_matrix");

	/* Cloud */
	printf("...Cloud shader\n");
	cloud.id = Shader::load(
		"shaders/cloud.vert",
		NULL,
		NULL,
		NULL,
		"shaders/cloud.frag"
	);
	glUseProgram(cloud.id);
	cloud.uniforms.model = glGetUniformLocation(cloud.id, "model");
	cloud.uniforms.model_view = glGetUniformLocation(cloud.id, "model_view");
	cloud.uniforms.model_view_perspective = glGetUniformLocation(cloud.id, "model_view_perspective");
	cloud.uniforms.noise_map = glGetUniformLocation(cloud.id, "noise_map");
	glUniform1i(cloud.uniforms.noise_map, 2);

	/* Debug frame buffer */
	printf("...Other shaders\n");
	debug_frame_buffer.id = Shader::load(
		"shaders/debug_frame_buffer.vert",
		NULL,
		NULL,
		NULL,
		"shaders/debug_frame_buffer.frag"
	);
	glUseProgram(debug_frame_buffer.id);
	debug_frame_buffer.uniforms.shadow_texture = glGetUniformLocation(debug_frame_buffer.id, "shadow_texture");
	glUniform1i(debug_frame_buffer.uniforms.shadow_texture, 1);
}

/// Empty destructor
Shader::~Shader() {
	/* Empty */
}

/// Load vertex and fragment shader and return the compiled program
GLuint Shader::load(
	const char *vertex_path,
    const char *tess_control_path,
    const char *tess_eval_path,
	const char *geometry_path,
	const char *fragment_path )
{
	try {
		GLuint vertex_shader = -1;
		GLuint tess_control_shader = -1;
		GLuint tess_eval_shader = -1;
		GLuint geometry_shader = -1;
		GLuint fragment_shader = -1;
		
		// Read shaders
		std::string vertex_shader_str = read_file(vertex_path);
		std::string tess_control_shader_str;
		if(tess_control_path) {
			tess_control_shader_str = read_file(tess_control_path);
		}
		std::string tess_eval_shader_str;
		if(tess_eval_path) {
			tess_eval_shader_str = read_file(tess_eval_path);
		}
		std::string geometry_shader_str;
		if(geometry_path) {
			geometry_shader_str = read_file(geometry_path);
		}
		std::string fragment_shader_str = read_file(fragment_path);

		GLint result = GL_FALSE;
		int log_length;

		vertex_shader = build_shader(GL_VERTEX_SHADER, vertex_shader_str);
		if(tess_control_path) {
			tess_control_shader = build_shader(GL_TESS_CONTROL_SHADER, tess_control_shader_str);
		}
		if(tess_eval_path) {
			tess_eval_shader = build_shader(GL_TESS_EVALUATION_SHADER, tess_eval_shader_str);
		}
		if(geometry_path) {
			geometry_shader = build_shader(GL_GEOMETRY_SHADER, geometry_shader_str);
		}
		fragment_shader = build_shader(GL_FRAGMENT_SHADER, fragment_shader_str);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		if(tess_control_path) {
			glAttachShader(program, tess_control_shader);
		}
		if(tess_eval_path) {
			glAttachShader(program, tess_eval_shader);
		}
		if(geometry_path) {
			glAttachShader(program, geometry_shader);
		}
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> program_error((log_length > 1) ? log_length : 1);
		glGetProgramInfoLog(program, log_length, NULL, &program_error[0]);
		if(log_length > 0) {
			std::cout << &program_error[0] << std::endl;
		}

		glDeleteShader(vertex_shader);
		if(tess_control_path) {
			glDeleteShader(tess_control_shader);
		}
		if(geometry_path) {
			glDeleteShader(geometry_shader);
		}
		glDeleteShader(fragment_shader);

		return program;
	} catch(std::exception &e) {
		std::cout << "Caught exception( {" << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}
}

/// Build shaders from strings containing shader source code
GLuint Shader::build_shader(GLenum shader_type, const std::string &shader_text) {
	GLuint shader = glCreateShader(shader_type);
	const char *file_data_str = shader_text.c_str();
	glShaderSource(shader, 1, &file_data_str, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		// Output the compile errors
		
		GLint info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		GLchar *info_log_str = new GLchar[info_log_length + 1];
		glGetShaderInfoLog(shader, info_log_length, NULL, info_log_str);

		const char *shader_type_str = NULL;
		switch(shader_type) {
			case GL_VERTEX_SHADER: shader_type_str = "vertex"; break;
			case GL_TESS_CONTROL_SHADER: shader_type_str = "tessellation control"; break;
			case GL_TESS_EVALUATION_SHADER: shader_type_str = "tessellation evaluation"; break;
			case GL_GEOMETRY_SHADER: shader_type_str = "geometry"; break;
			case GL_FRAGMENT_SHADER: shader_type_str = "fragment"; break;
		}

		std::cerr << "Compile error in " << shader_type_str << "\n\t" << info_log_str << std::endl;
		delete[] info_log_str;

		throw std::runtime_error("Shader compile exception");
	}

	return shader;
}

/// Read a text file into a string
std::string Shader::read_file(const char *file_path) {
	std::string content;
	std::ifstream file_stream(file_path, std::ios::in);

	if(!file_stream.is_open()) {
		std::cerr << "Could not read file " << file_path << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!file_stream.eof()) {
		std::getline(file_stream, line);
		content.append(line + "\n");
	}

	file_stream.close();
	return content;
}
