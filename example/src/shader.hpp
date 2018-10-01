#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include "GL/glew.h"

/// Struct used to store the IDs of uniforms to be sent to shaders
struct ShaderUniforms {
    /// view uniform
	GLuint view;
    /// view projection uniform
    GLuint view_projection;
    /// model uniform
    GLuint model;
    /// model view uniform
    GLuint model_view;
    /// model view perspective uniform
    GLuint model_view_perspective;
    /// normal matrix uniform
    GLuint normal_matrix;
    /// depth matrix uniform
    GLuint depth_matrix;
    /// light view position uniform
    GLuint light_camera_position;
    /// light position uniform
    GLuint light_position;
    /// view direction uniform
    GLuint view_direction;
    /// emissive light uniform
    GLuint emissive_light;
    /// shininess uniform
    GLuint shininess;
    /// textured flag uniform
    GLuint textured;
    /// texture uniform
    GLuint texture;
    /// shadow texture uniform
    GLuint shadow_texture;
    /// view position uniform
    GLuint camera_position;
    /// camera focal length uniform
    GLuint camera_focal_length;
    /// window size uniform
    GLuint window_size;
    /// roughness uniform
    GLuint roughness;
    /// time uniform
    GLuint time;
    /// min vertex height uniform
    GLuint min_vertex_height;
    /// max vertex height uniform
    GLuint max_vertex_height;
    /// min vertex magnitude uniform
    GLuint min_vertex_magnitude;
    /// max vertex magnitude uniform
    GLuint max_vertex_magnitude;
    /// noise map uniform
    GLuint noise_map;
    /// sand texture uniform
    GLuint sand_texture;
    /// grass texture uniform
    GLuint grass_texture;
    /// dark grass texture uniform
    GLuint dark_grass_texture;
    /// dirt texture uniform
    GLuint dirt_texture;
    /// dark dirt texture uniform
    GLuint dark_dirt_texture;
    /// snow texture uniform
    GLuint snow_texture;
};

/// Stores information about a specific shader

/// Shader program ID and uniforms struct
struct ShaderProgram {
    /// Shader program ID
    GLuint id;
    /// Store shader uniforms
    ShaderUniforms uniforms;
};

/// Class for dealing with shaders and uniforms
class Shader {
public:
    /// Blinn-Phong shader
    ShaderProgram blinn_phong;
    /// Oren-Nayar shader
    ShaderProgram oren_nayar;
    /// Skybox shader
    ShaderProgram skybox;
    /// Shadow shader
    ShaderProgram shadow;
    /// Cloud shader
    ShaderProgram cloud;
    /// Debug frame buffer shader (shadow map viewer)
    ShaderProgram debug_frame_buffer;

	Shader();
	~Shader();
private:
    /// Contains static functions used to load shader programs \n
    /// Credit: Iain Martin - August 2014
	static GLuint build_shader(GLenum shader_type, const std::string &shader_text);
	static std::string read_file(const char *file_path);
    static GLuint load(
        const char *vertex_path,
        const char *tess_control_path,
        const char *tess_eval_path,
        const char *geometry_path,
        const char *fragment_path
    );
};

#endif /* SHADER_HPP */
