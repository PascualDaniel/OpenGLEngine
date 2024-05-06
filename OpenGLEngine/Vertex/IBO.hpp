#ifndef IBO_HPP
#define IBO_HPP

#include<glad/glad.h>

class IBO
{
public:

	GLuint ID;
	
	IBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};


#endif