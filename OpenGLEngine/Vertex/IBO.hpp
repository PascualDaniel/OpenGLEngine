#ifndef IBO_HPP
#define IBO_HPP

#include<glad/glad.h>
#include <vector>

class IBO
{
public:

	GLuint ID;
	IBO();
	IBO(std::vector<GLuint> indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};


#endif