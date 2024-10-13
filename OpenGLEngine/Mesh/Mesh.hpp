#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"../VAO/VAO.hpp"
#include"../EBO/EBO.hpp"
#include"../Camera/Camera.hpp"

struct Transform {
	glm::mat4 mModelMatrix{ glm::mat4(1.0f) };
};


class Mesh
{
public:
	std::vector <GLfloat> vertices;
	std::vector <GLuint> indices;

	// Store VAO in public so it can be used in the Draw function
	VAO VAO;

	//graphic pipeline usado con el mesh
	GLuint mPipeline = 0;

	Transform mTransform;

	float m_uOffset = -2.0f;


	// Initializes the mesh
	Mesh(std::vector <GLfloat>& vertices, std::vector <GLuint>& indices);

	// Draws the mesh
	void Draw(GLuint& shader, Camera& camera);
};
#endif
