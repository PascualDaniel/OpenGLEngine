#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP


#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include "../VBO/VBO.hpp"


class VAO {
public:
	GLuint ID;
	VAO();
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	void Bind();
	void Unbind();
	void Delete();
};

#endif

