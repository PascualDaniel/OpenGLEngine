#ifndef EBO_CLASS_HPP
#define EBO_CLASS_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <vector>
class EBO {
public:
	GLuint ID;
	EBO(const std::vector <GLuint>& indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

