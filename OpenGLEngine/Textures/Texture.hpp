#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include <iostream>
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "stb_image.h"

#include "Shader.hpp"


class Texture {
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	

	Texture(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

};


#endif