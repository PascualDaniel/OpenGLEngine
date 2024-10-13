#include "Mesh.hpp"
#include <iostream>


Mesh::Mesh(std::vector <GLfloat>& vertices, std::vector <GLuint>& indices)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(GLfloat), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(GLfloat), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(GLfloat), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw(GLuint& shader, Camera& mCamera)
{
    glUseProgram(mPipeline);

    //Devuelve la localizacion de la matriz del modelo
    GLint u_ModelMatrixLocation = FindUniformLocation(shader, "u_ModelMatrix");
    glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, mTransform.mModelMatrix[0][0]);

    // Camara, controla la matriz de la camara
    glm::mat4 view = mCamera.GetViewMatrix();
    GLint u_ViewLocation = FindUniformLocation(shader, "u_ViewMatrix");
    glUniformMatrix4fv(u_ViewLocation, 1, false, &view[0][0]);

    //Projection Transformation 
    glm::mat4 perspective = mCamera.GetProjectionMatrix();

    //Devuelve la localizacion de la perspectiva
    GLint u_ProjectionLocation = FindUniformLocation(shader, "u_Projection");
    glUniformMatrix4fv(u_ProjectionLocation, 1, false, &perspective[0][0]);

    //Activa atributos
    VAO.Bind();
    //Renderiza los datos
    //#define GLCheck(x) GLClearAllErrors(); x;  GLCheckErrorStatus(#x,__LINE__);
    //GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //Para de usar el pipeline (Necesario si solo hay un pipeline)
    glUseProgram(0);
}

int FindUniformLocation(GLuint pipeline, const GLchar* name) {
    //Devuelve la localizacion de la matriz
    GLint location = glGetUniformLocation(pipeline, name);
    if (location < 0) {
        std::cerr << "Could not find  " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return location;
}