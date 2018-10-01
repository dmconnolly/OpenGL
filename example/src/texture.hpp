#pragma once
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <string>

/// Used for managing and loading textures

/// Stores map containing previously loaded textures
/// Functions to allow loading of new textures
class Texture {
public:
    Texture();
	~Texture();
	GLuint load(std::string texture_path);
	GLuint load_cubemap(std::string texture_paths[6]);
private:
	/// Map of loaded texture file paths with their associated IDs \n
	/// std::string is used instead of char* so that the
	/// map comparison function works correctly
	std::map<std::string, GLuint> loaded_id;
};

#endif /* TEXTURE_HPP */
