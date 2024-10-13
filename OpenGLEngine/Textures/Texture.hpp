#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "Shader.hpp"
#include "stb_image.h"
#include <iostream>

class Texture {
public:
  
	GLuint ID;


	void CreateTexture(GLuint& texture, const char* filePath);
	void DeleteTexture(GLuint& texture);

};


#endif