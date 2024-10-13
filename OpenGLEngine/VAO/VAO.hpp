#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP


#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "../VBO/VBO.hpp"


class VAO {
public:
	GLuint ID;
	VAO();
	void LinkVBO(VBO& VBO, GLuint layout);

	void Bind();
	void Unbind();
	void Delete();
};

#endif

