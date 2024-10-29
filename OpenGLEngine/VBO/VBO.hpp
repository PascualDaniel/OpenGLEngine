#ifndef VBO_CLASS_HPP
#define VBO_CLASS_HPP

#include<glm/glm.hpp>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <vector>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO {
public:
	GLuint ID;
	VBO(const std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

