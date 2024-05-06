#ifndef VBO_HPP
#define VBO_HPP

#include<glad/glad.h>
#include <vector>
class VBO
{
public:
	GLuint ID;
	VBO();
	VBO(std::vector<GLfloat>, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();
};

#endif