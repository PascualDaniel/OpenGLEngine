// Mesh3D.cpp
#include "Mesh.hpp"
#include <iostream>


Mesh::Mesh(std::vector <GLfloat>& vertices, std::vector <GLuint>& indices, Texture& texture)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::texture = texture;

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO EBO(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0); // Posición
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Coordenadas de textura

    VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(vertices), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}



void Mesh::Delete() {
    //glDeleteBuffers(1, &mVertexArrayObject);
    //glDeleteVertexArrays(1, &mVertexArrayObject);
	VAO.Delete();
}

void Mesh::Translate( float x, float y, float z) {
    mTransform.mModelMatrix = glm::translate(mTransform.mModelMatrix, glm::vec3(x, y, z));
}

void Mesh::Rotate( float angle, glm::vec3 axis) {
    mTransform.mModelMatrix = glm::rotate(mTransform.mModelMatrix, glm::radians(angle), axis);
}

void Mesh::Scale( float x, float y, float z) {
    mTransform.mModelMatrix = glm::scale(mTransform.mModelMatrix, glm::vec3(x, y, z));
}


/**
* Devuelve al localizacion de una variable uniforme basada en su nombre
*/
int FindUniformLocation(GLuint pipeline, const GLchar* name) {
    //Devuelve la localizacion de la matriz
    GLint location = glGetUniformLocation(pipeline, name);
    if (location < 0) {
        std::cerr << "Could not find  " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return location;
}

void Mesh::Draw(const Camera& camera) {

    

    glUseProgram(mPipeline);

    GLint u_ModelMatrixLocation = glGetUniformLocation(mPipeline, "u_ModelMatrix");
    glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, &mTransform.mModelMatrix[0][0]);

    // Camara, controla la matriz de la camara
    glm::mat4 view = camera.GetViewMatrix();
    GLint u_ViewLocation = FindUniformLocation(mPipeline, "u_ViewMatrix");
    glUniformMatrix4fv(u_ViewLocation, 1, false, &view[0][0]);

    //Projection Transformation 
    glm::mat4 perspective = camera.GetProjectionMatrix();

    //Devuelve la localizacion de la perspectiva
    GLint u_ProjectionLocation = FindUniformLocation(mPipeline, "u_Projection");
    glUniformMatrix4fv(u_ProjectionLocation, 1, false, &perspective[0][0]);



    
	VAO.Bind();
    //Textura
	texture.texUnit(mPipeline, "u_Tex0", 0);
	texture.Bind();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


	

    //Para de usar el pipeline (Necesario si solo hay un pipeline)
    glUseProgram(0);
    texture.Unbind();
    VAO.Unbind();
}

void Mesh::SetPipeline( GLuint pipeline) {
    mPipeline = pipeline;
}
