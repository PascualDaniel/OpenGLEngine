#include "Shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

    vertexShaderSource = LoadShaderAsString("../shaders/vert.glsl");
    fragmentShaderSource = LoadShaderAsString("../shaders/frag.glsl");

    gGraphicsPipeline =  CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void Shader::use() {
    glUseProgram(gGraphicsPipeline);
}
void Shader::drop() {
	glDeleteProgram(gGraphicsPipeline);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(gGraphicsPipeline, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(gGraphicsPipeline, name.c_str()), value);

}

GLuint Shader::getGraphicsPipeline() {
    if (gGraphicsPipeline != 0) {
        return gGraphicsPipeline;
    }
    else {
        exit(1);
    }
}


std::string Shader::LoadShaderAsString(const std::string& filename) {
    //Resulting shader program as as single string
    std::string result = "";
    std::string line = "";
    std::ifstream myFile(filename.c_str());

    if (myFile.is_open()) {
        while (std::getline(myFile, line)) {
            result += line + '\n';
        }
        myFile.close();
    }

    return result;

}

GLuint Shader::CompileShader(GLuint type, const std::string& shadersource) {
    GLuint shaderObject;
    //Crea un shader dependiendo del tipo
    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* src = shadersource.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    //Devuelve el resultado de la compilación
    int result;
    //Devolver el estatus
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length]; //Se puede usar alloca aqui
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

        if (type == GL_VERTEX_SHADER) {
            std::cout << "Error: GL_VERTEX_SHADER compilation failed.\n" << errorMessages << std::endl;
        }
        else if (type == GL_FRAGMENT_SHADER) {
            std::cout << "Error: GL_FRAGMENT_SHADER compilation failed.\n" << errorMessages << std::endl;
        }
        //Reclaim memory
        delete[] errorMessages;

        //Delete broken shaders
        glDeleteShader(shaderObject);

        return 0;
    }

    return shaderObject;
}

GLuint Shader::CreateShaderProgram(const std::string& vertexshaderssource, const std::string& fragmentshadersource) {
    //Create graphics pipeline
    GLuint programObject = glCreateProgram();

    //Compila los shaders
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshaderssource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    //Validate
    glValidateProgram(programObject);
    //Detach Shaders

    return programObject;

}

