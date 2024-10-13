// Mesh3D.hpp
#ifndef MESH3D_HPP
#define MESH3D_HPP

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

class Mesh {
public:
    void Create();
    void Delete();
    void Translate(float x, float y, float z);
    void Rotate(float angle, glm::vec3 axis);
    void Scale(float x, float y, float z);
    void Draw(const Camera& camera);
    void SetPipeline(GLuint pipeline);
    float m_uOffset;

private:
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mIndexBufferObject;
    GLuint mPipeline;
    
    struct Transform {
        glm::mat4 mModelMatrix = glm::mat4(1.0f);
    } mTransform;
};
#endif // MESH3D_HPP