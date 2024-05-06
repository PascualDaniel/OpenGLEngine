#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath); // constructor
    void use(); // method to use the shader
    void setInt(const std::string& name, int value); // method to set int uniform
    void setFloat(const std::string& name, float value); // method to set float uniform
    
    GLuint getGraphicsPipeline();

    void Delete(); // Desvincula el shader
    

private:
    unsigned int ID; // ID of the shader
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    // Program Object for our shaders (Graphics pipeline)
    GLuint gGraphicsPipeline = 0;


    std::string LoadShaderAsString(const std::string& filename);
    GLuint CompileShader(GLuint type, const std::string& shadersource);
    GLuint CreateShaderProgram(const std::string& vertexshaderssource, const std::string& fragmentshadersource);
};



#endif