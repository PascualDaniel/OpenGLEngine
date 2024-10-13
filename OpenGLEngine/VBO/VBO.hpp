#ifndef VBO_CLASS_HPP
#define VBO_CLASS_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <vector>
class VBO {
public:
	GLuint ID;
	VBO(const std::vector<GLfloat>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

