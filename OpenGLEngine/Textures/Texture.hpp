#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
public:
  
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);


	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	void Delete();
};


#endif