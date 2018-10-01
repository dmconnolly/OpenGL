#include <stdio.h>
#include <map>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "soil/soil.h"

#include "texture.hpp"

/// Empty constructor
Texture::Texture() {
	/* Empty */
}

/// Empty destructor
Texture::~Texture() {
	/* Empty */
}

/// Loads a texture file and returns an ID

/// Loads OpenGL texture using soil library \n
/// Newly loaded texture is bound and initialised \n
/// The new texture ID is stored in the map and returned \n
/// If the texture was previously loaded, it is accessed through the map and returned
GLuint Texture::load(std::string texture_path) {
	std::map<std::string, GLuint>::iterator it = loaded_id.find(texture_path);

	/* If texture was already loaded */
	if(it != loaded_id.end()) {
		/* Return the ID */
	    return it->second;
	}

	try {
		/* Load image file as a new OpenGL texture */
		GLuint texture_id = SOIL_load_OGL_texture(
			texture_path.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | 
				SOIL_FLAG_INVERT_Y | 
				SOIL_FLAG_NTSC_SAFE_RGB | 
				SOIL_FLAG_COMPRESS_TO_DXT
		);

		/* Check for success */
		if(!texture_id) {
			printf("Failed to load '%s'\n", texture_path.c_str());
			printf("Error: '%s'\n", SOIL_last_result());
			return -1;
		}

		/* Bind the new texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		/* Define texture behaviour */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Store newly loaded texture in map
		loaded_id[texture_path] = texture_id;

		return texture_id;
	} catch(std::exception &e) {
		printf("Texture file loading failed.\n");
		return -1;
	}
}

/// Loads texture for a cubemap (used for skybox)

/// Takes a pointer to a c-string array for the paths of 6 textures \n
/// These are in the order: right; left; top; bottom; back; front \n
/// Each texture is loaded using the soil library and initialised \n
/// The ID for the new texture is returned
GLuint Texture::load_cubemap(std::string texture_paths[6]) {
	try {
		/* Load textures */
	    GLuint texture_id = SOIL_load_OGL_cubemap (
			texture_paths[0].c_str(),
			texture_paths[1].c_str(),
			texture_paths[2].c_str(),
			texture_paths[3].c_str(),
			texture_paths[4].c_str(),
			texture_paths[5].c_str(),
			SOIL_LOAD_RGB,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
		);

	    /* Check for success */
		if(!texture_id) {
			printf("Failed to load cubemap texture\n");
			printf("Error: '%s'\n", SOIL_last_result());
			return -1;
		}

		/* Bind the new texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

		/* Define texture behaviour */
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	    return texture_id;
	} catch(std::exception &e) {
		printf("Cubemap texture loading failed.\n");
		return -1;
	}
}
