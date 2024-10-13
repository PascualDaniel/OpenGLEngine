#ifndef EBO_CLASS_HPP
#define EBO_CLASS_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class EBO {
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

