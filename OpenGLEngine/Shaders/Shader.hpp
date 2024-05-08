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
    Shader(const char* vertexFile, const char* fragmentFile); // constructor
    void use(); // method to use the shader
    void setInt(const std::string& name, int value); // method to set int uniform
    void setFloat(const std::string& name, float value); // method to set float uniform
    

    void Activate(); // Desvincula el shader
    void Delete(); // Desvincula el shader
    

private:
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    // Program Object for our shaders (Graphics pipeline)
    GLuint gGraphicsPipeline = 0;




    void compileErrors(unsigned int shader, const char* type);
};



#endif