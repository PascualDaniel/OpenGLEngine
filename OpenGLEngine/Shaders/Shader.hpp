#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // Reference ID of the Shader Program
    GLuint ID;
    Shader();
    Shader(const std::string& vertexPath, const std::string& fragmentPath); // constructor
    void use(); // method to use the shader
    void setInt(const std::string& name, int value); // method to set int uniform
    void setFloat(const std::string& name, float value); // method to set float uniform
    
    GLuint getGraphicsPipeline();
    void Activate(); // Desvincula el shader
    void Delete(); // Desvincula el shader
    

private:
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    // Program Object for our shaders (Graphics pipeline)
    GLuint gGraphicsPipeline = 0;


    std::string LoadShaderAsString(const std::string& filename);
    GLuint CompileShader(GLuint type, const std::string& shadersource);
    GLuint CreateShaderProgram(const std::string& vertexshaderssource, const std::string& fragmentshadersource);
};



#endif