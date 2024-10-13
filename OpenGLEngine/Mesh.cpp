// Mesh3D.cpp
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
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(vertices), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(vertices), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(vertices), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Create() {
    const std::vector<GLfloat> vertexData
    { //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
    0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
    0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
    };

    const std::vector<GLuint> indexBufferData
    {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    

    glGenBuffers(1, &mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 6));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //Para de usar el pipeline (Necesario si solo hay un pipeline)
    glUseProgram(0);

    VAO.Unbind();
}

void Mesh::SetPipeline( GLuint pipeline) {
    mPipeline = pipeline;
}
